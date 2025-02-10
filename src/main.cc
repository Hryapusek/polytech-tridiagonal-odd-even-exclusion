#include <iostream>
#include <iomanip>  // For std::setw, std::fixed, std::setprecision, etc.
#include <chrono>

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <defines.hpp>
#include <input_parameters.hpp>
#include <default_impl/main_matrix_calculator.hpp>
#include <interval_splitter.hpp>

auto build_main_matrix(
  DefaultMainMatrixCalculator const& calc,
  double t,
  Eigen::MatrixXd& main_matrix
) -> Eigen::MatrixXd
{
  auto const size = calc.r_points().size() - 1;
  contract(fun) { precondition(main_matrix.rows() == size and main_matrix.cols() == size); }

  for(size_t row = 0; row < size; ++row) {
    main_matrix(row, row) = calc.calc_c(row + 1, t);

    if(row + 1 < size) {
      main_matrix(row, row + 1) = calc.calc_b(row + 1, t);
    }

    if(row > 0) {
      main_matrix(row, row - 1) = calc.calc_a(row + 1, t);
    }
  }

  return main_matrix;
}

auto build_g_vector(DefaultMainMatrixCalculator const& calc, double t)
  -> Eigen::SparseVector<double>
{
  auto g = Eigen::SparseVector<double>(calc.r_points().size() - 1);
  for(size_t row = 0; row < calc.r_points().size() - 1; ++row) {
    g.insert(row) = calc.calc_g(row + 1, t);
  }
  return g;
}

void do_all(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func)
{
  static constexpr auto x_interval_counts = {100, 500, 1'000, 2'000, 5'000, 10'000};
  static constexpr auto y_interval_counts = {5, 10, 20, 50, 100};

  for(auto const x_count : x_interval_counts) {
    for(auto const y_count : y_interval_counts) {
      auto x_points = split_interval(params->xl, params->xr, x_count);
      auto y_points = split_interval(params->yl, params->yr, y_count);

      DefaultMainMatrixCalculator calc(params, x_points);

      
    }
    std::cout << std::string(50, '-') << std::endl;
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

  params->u2 = [](double y) { return 15000 + 10 * y*y*y + 1800; };

  params->f = [](double x, double y) { return -36 * x - 12 * y; };

  auto expected_func = [](double r, double t) { return t + 2 * r; };

  do_all(params, expected_func);
}

int main()
{
  basic_example();
  return 0;
}
