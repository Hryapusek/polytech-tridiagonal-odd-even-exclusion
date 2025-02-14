#include <iostream>
#include <iomanip>  // For std::setw, std::fixed, std::setprecision, etc.
#include <chrono>
#include <memory>

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <defines.hpp>
#include <input_parameters.hpp>
#include <default_impl/main_matrix_calculator.hpp>
#include <interval_splitter.hpp>
#include <default_impl/odd_even_reduction.hpp>
#include <utils.hpp>

auto build_main_matrix(DefaultMainMatrixCalculator const& calc) -> Eigen::MatrixXd
{
  static auto cached_matrix = std::shared_ptr<Eigen::MatrixXd> {};

  if (cached_matrix) {
    return *cached_matrix;
  }

  size_t Nx = calc.interiour_x_points().size();  // Interior points in x-direction
  size_t Ny = calc.interiour_y_points().size();  // Interior points in y-direction
  size_t size = Nx * Ny;  // Total unknowns (interior grid points)

  cached_matrix = std::make_shared<Eigen::MatrixXd>(size, size);
  cached_matrix->setZero();  // Initialize with zeros

  // Iterate over the interior grid
  for (size_t i = 0; i < Nx; ++i) {    
    for (size_t j = 0; j < Ny; ++j) {  
      size_t idx = i * Ny + j;  // Correct 1D index

      // Left neighbor (i-1, j)
      if (i > 0) {
        (*cached_matrix)(idx, idx - Ny) = calc.calc_a({i, j});
      }

      // Right neighbor (i+1, j)
      if (i < Nx - 1) {
        (*cached_matrix)(idx, idx + Ny) = calc.calc_b({i, j});
      }

      // Bottom neighbor (i, j-1)
      if (j > 0) {
        (*cached_matrix)(idx, idx - 1) = calc.calc_d({i, j});
      }

      // Top neighbor (i, j+1)
      if (j < Ny - 1) {
        (*cached_matrix)(idx, idx + 1) = calc.calc_e({i, j});
      }

      // Center coefficient
      (*cached_matrix)(idx, idx) = calc.calc_c({i, j});
    }
  }

  return *cached_matrix;
}


auto build_g_vector(DefaultMainMatrixCalculator const& calc) -> Eigen::VectorXd
{
  size_t Nx = calc.interiour_x_points().size();  // Interior points in x-direction
  size_t Ny = calc.interiour_y_points().size();  // Interior points in y-direction
  size_t size = Nx * Ny;  // Total number of unknowns

  Eigen::VectorXd g(size);  // Initialize vector of correct size

  // Iterate over interior grid points
  for (size_t i = 0; i < Nx; ++i) {    
    for (size_t j = 0; j < Ny; ++j) {  
      size_t idx = i * Ny + j;  // Convert (i, j) to 1D index
      g(idx) = calc.calc_g({i, j});  // Compute g at (i, j)
    }
  }

  return g;
}

auto reduce_matrix(DefaultMainMatrixCalculator const& calc, Eigen::MatrixXd const& matrix)
  -> Eigen::MatrixXd
{
  Eigen::MatrixXd reduced_matrix(matrix.rows() - 2, matrix.cols());
  for(size_t row = 1; row < calc.x_points().size(); ++row) {
    reduced_matrix.row(
      row + calc.x_points().size()
    ) = reduced_matrix.row(row + calc.x_points().size())
      - matrix.row(row) * (-matrix(row + calc.x_points().size(), row));
  }

  for(size_t row = matrix.rows() - calc.x_points().size(); row < matrix.rows(); ++row) {
    reduced_matrix.row(
      row - calc.x_points().size()
    ) = reduced_matrix.row(row - calc.x_points().size())
      - matrix.row(row) * (-matrix(row - calc.x_points().size(), row));
  }

  return reduced_matrix;
}

auto convert_w_to_v(Eigen::VectorXd const& w, DefaultMainMatrixCalculator const& calc) -> Eigen::MatrixXd {
    size_t Nx = calc.interiour_x_points().size();
    size_t Ny = calc.interiour_y_points().size();

    // Full grid size (includes boundaries)
    Eigen::MatrixXd v(Nx + 2, Ny + 2);
    v.setZero();  // Initialize with zeros

    // 🔹 Map interior points from w back to v
    size_t idx = 0;
    for (size_t i = 1; i <= Nx; ++i) {  // Skip first & last row (boundaries)
        for (size_t j = 1; j <= Ny; ++j) {  // Skip first & last column (boundaries)
            v(i, j) = w(idx++);  // Fill interior solution
        }
    }

    // 🔹 Apply boundary conditions using input parameters
    auto params = calc.params();

    // Left boundary (x = xl, using u1)
    for (size_t j = 0; j < Ny + 2; ++j) {
        v(0, j) = params->u1(calc.y_points()[j]);
    }

    // Right boundary (x = xr, using u2)
    for (size_t j = 0; j < Ny + 2; ++j) {
        v(Nx + 1, j) = params->u2(calc.y_points()[j]);
    }

    // Bottom boundary (y = yl, using u3)
    for (size_t i = 0; i < Nx + 2; ++i) {
        v(i, 0) = params->u3(calc.x_points()[i]);
    }

    // Top boundary (y = yr, using u4)
    for (size_t i = 0; i < Nx + 2; ++i) {
        v(i, Ny + 1) = params->u4(calc.x_points()[i]);
    }

    return v;  // ✅ Return reconstructed solution grid
}


void print_expected(DefaultMainMatrixCalculator const& calc, X_Y_Function_type expected_func) {
    const auto& x_points = calc.x_points();  // Full x grid
    const auto& y_points = calc.y_points();  // Full y grid
    size_t Nx = x_points.size();
    size_t Ny = y_points.size();

    // 🔹 Print table header (y-values at top)
    // 🔹 Print computed values in a grid format
    for (size_t i = 0; i < Nx; ++i) {
        for (size_t j = 0; j < Ny; ++j) {
            double expected_value = expected_func(x_points[i], y_points[j]);  // Compute expected value
            std::cout << std::setw(10) << std::fixed << std::setprecision(4) << expected_value;
        }
        std::cout << "\n";
    }
}

void _do_all(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func)
{
  static constexpr auto x_interval_counts = {10};
  static constexpr auto y_interval_counts = {10};

  for(auto const x_count : x_interval_counts) {
    for(auto const y_count : y_interval_counts) {
      auto x_points = split_interval(params->xl, params->xr, x_count);
      auto y_points = split_interval(params->yl, params->yr, y_count);

      DefaultMainMatrixCalculator calc(params, x_points, y_points);
      auto main_matrix = build_main_matrix(calc);
      std::cout << "Main matrix: \n" << main_matrix << '\n';
      std::cout << "----------------------------------------\n";
      auto g_vector = build_g_vector(calc);
      std::cout << "G vector: \n" << g_vector << '\n';
      std::cout << "----------------------------------------\n";
      std::cout << "Main matrix size: " << main_matrix.rows() << "x" << main_matrix.cols() << '\n';
      std::cout << "G vector size: " << g_vector.size() << '\n';
      Eigen::MatrixXd solution = main_matrix.fullPivLu().solve(g_vector);
      std::cout << "Solution: \n" << solution << '\n';
      auto v_matrix = convert_w_to_v(solution, calc);
      std::cout << "Solution in v coordinates: \n" << v_matrix << '\n';
      std::cout << "----------------------------------------\n";
      std::cout << "Expected:: \n";
      print_expected(calc, expected_func);
    }
  }
}

void basic_example()
{
  std::shared_ptr<InputParameters> params = std::make_shared<InputParameters>();
  params->xl = 1;
  params->xr = 10;
  params->yl = 1;
  params->yr = 5;

  params->u1 = [](double y) { return 3 + 2 * y * y * y; };
  params->u3 = [](double x) { return 3 * x * x * x + 2; };
  params->u4 = [](double x) { return 3 * x * x * x + 250; };

  params->k1 = [](double) { return 2; };
  params->hi2 = 5;

  params->u2 = [](double y) { return 15'000 + 10 * y * y * y + 1'800; };

  params->f = [](double x, double y) { return -36 * x - 12 * y; };

  auto expected_func = [](double x, double y) { return 3 * x * x * x + 2 * y * y * y; };

  _do_all(params, expected_func);
}

int main()
{
  basic_example();
  return 0;
}
