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

auto build_main_matrix(
  DefaultMainMatrixCalculator const& calc
) -> Eigen::MatrixXd
{
  static auto cached_matrix = std::shared_ptr<Eigen::MatrixXd>{};

  if (cached_matrix) {
    return *cached_matrix;
  }

  cached_matrix = std::make_shared<Eigen::MatrixXd>(calc.x_points().size(), calc.y_points().size());

  for(size_t row = 0; row < std::min(calc.x_points().size(), calc.y_points().size()); ++row) {
    if (row > 0) {
      (*cached_matrix)(row, row - 1) = calc.calc_a({row, row - 1});
    }

    if (row < calc.x_points().size() - 1) {
      (*cached_matrix)(row, row + 1) = calc.calc_b({row, row + 1});
    }

    if (row > 4) {
      (*cached_matrix)(row, row - 5) = calc.calc_d({row, row - 5});
    }

    if (row < calc.x_points().size() - 5) {
      (*cached_matrix)(row, row + 5) = calc.calc_e({row, row + 5});
    }

    (*cached_matrix)(row, row) = calc.calc_c({row, row});
  }

  return *cached_matrix;
}

auto build_g_vector(DefaultMainMatrixCalculator const& calc, size_t x_index)
  -> Eigen::VectorXd
{
  auto rows = calc.x_points().size() * calc.y_points().size();
  auto g = Eigen::SparseVector<double>(rows);
  for(size_t row = calc.x_points().size() + 1; row < rows - calc.x_points().size(); ++row) {
    g.insert(row) = calc.calc_g(Index{x_index, row});
  }
  return g;
}

auto reduce_matrix(DefaultMainMatrixCalculator const& calc, Eigen::MatrixXd const& matrix) -> Eigen::MatrixXd {
  Eigen::MatrixXd reduced_matrix(matrix.rows() - 2, matrix.cols());
  for(size_t row = 1; row < calc.x_points().size(); ++row) {
    reduced_matrix.row(row + calc.x_points().size()) = reduced_matrix.row(row + calc.x_points().size()) - matrix.row(row) * (-matrix(row + calc.x_points().size(), row));
  }

  for (size_t row = matrix.rows() - calc.x_points().size(); row < matrix.rows(); ++row) {
    reduced_matrix.row(row - calc.x_points().size()) = reduced_matrix.row(row - calc.x_points().size()) - matrix.row(row) * (-matrix(row - calc.x_points().size(), row));
  }

  return reduced_matrix;
}

void print_table(Eigen::MatrixXd const& table) {}

void _do_all(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func)
{
  static constexpr auto x_interval_counts = {5};
  static constexpr auto y_interval_counts = {5};

  for(auto const x_count : x_interval_counts) {
    for(auto const y_count : y_interval_counts) {
      auto x_points = split_interval(params->xl, params->xr, x_count);
      auto y_points = split_interval(params->yl, params->yr, y_count);
      auto size = x_points.size() * y_points.size();
      Eigen::MatrixXd result_table = Eigen::MatrixXd::Zero(size, size);

      DefaultMainMatrixCalculator calc(params, x_points, y_points);
      auto main_matrix = build_main_matrix(calc);
      auto main_matrix_reduced = reduce_matrix(calc, main_matrix);

      for (size_t i = 0; i < x_points.size(); ++i) {
        auto g_vector = build_g_vector(calc, x_points[i]);
        auto result = odd_even_reduction_solver(main_matrix_reduced, g_vector);
        result_table.col(i) = result;
      }
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

  do_all1(params, expected_func);
}

int main()
{
  basic_example();
  return 0;
}
