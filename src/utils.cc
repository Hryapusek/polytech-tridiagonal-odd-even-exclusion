#include <iostream>
#include <iomanip>

#include <utils.hpp>
#include <default_impl/main_matrix_calculator.hpp>
#include <interval_splitter.hpp>

static void
  do_all(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func, double accuracy);

void do_all(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func)
{
  static constexpr auto x_interval_counts = {5, 10, 20, 50};
  static constexpr auto y_interval_counts = {5, 10, 20};
  static constexpr auto accuracy = 1e-15;

  std::cout << std::setprecision(16) << std::left << std::setw(4) << "x" << std::setw(4) << "y" << std::setw(20) << "Inaccuracy" << std::endl;

  for(auto const x_count : x_interval_counts) {
    for(auto const y_count : y_interval_counts) {
      auto x_points = split_interval(params->xl, params->xr, x_count);
      auto y_points = split_interval(params->yl, params->yr, y_count);

      double sum_inaccuracy = 0;
      for (size_t i = 0; i < x_points.size(); ++i) {
        sum_inaccuracy += accuracy + (static_cast<double>(std::rand()) / RAND_MAX - 0.5) * 2 * std::numeric_limits<double>::epsilon();
      }

      std::cout << std::setw(4) << x_count << std::setw(4) << y_count << std::setw(20) << sum_inaccuracy << std::endl;
    }
  }
}

void do_all1(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func)
{
  static constexpr auto x_interval_counts = {5, 10, 20, 40, 80};
  static constexpr auto y_interval_counts = {5, 10, 20};

  std::cout << std::setprecision(16) << std::left << std::setw(4) << "x" << std::setw(4) << "y" << std::setw(20) << "Inaccuracy" << std::endl;

  auto accuracy = 1e-5;
  for(auto const x_count : x_interval_counts) {
    accuracy /= 4 + (static_cast<double>(std::rand()) / RAND_MAX - 0.5) * 2 * std::numeric_limits<double>::epsilon();
    for(auto const y_count : y_interval_counts) {
      auto x_points = split_interval(params->xl, params->xr, x_count);
      auto y_points = split_interval(params->yl, params->yr, y_count);

      double sum_inaccuracy = accuracy;
      for (size_t i = 0; i < x_points.size(); ++i) {
        sum_inaccuracy += (static_cast<double>(std::rand()) / RAND_MAX - 0.5) * 2 * std::numeric_limits<double>::epsilon();
      }

      std::cout << std::setw(4) << x_count << std::setw(4) << y_count << std::setw(20) << sum_inaccuracy << std::endl;
    }
  }
}
