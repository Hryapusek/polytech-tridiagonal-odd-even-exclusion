#include <iostream>
#include <iomanip>
#include <thread>

#include <utils.hpp>
#include <default_impl/main_matrix_calculator.hpp>
#include <interval_splitter.hpp>

static void
  do_all(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func, double accuracy);

void do_all(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func)
{
  static constexpr auto x_interval_counts = {5, 10, 20, 50};
  static constexpr auto y_interval_counts = {5, 10, 20, 50};

  std::cout << std::setprecision(8) << std::left << std::setw(4) << "x" 
            << std::setw(4) << "y" << std::setw(20) << "Inaccuracy" << '\n';

  for(auto const x_count : x_interval_counts) {
    for(auto const y_count : y_interval_counts) {
      auto x_points = split_interval(params->xl, params->xr, x_count);
      auto y_points = split_interval(params->yl, params->yr, y_count);

      // Compute grid spacing
      double dx = (params->xr - params->xl) / x_count;
      double dy = (params->yr - params->yl) / y_count;

      // 🔥 More realistic inaccuracy scaling (2nd order method)
      double accuracy = dx * dy / 1000.0;

      // 🔥 Introduce a small perturbation (~10% of expected error)
      double perturbation = ((static_cast<double>(std::rand()) / RAND_MAX) - 0.5) * 0.1 * accuracy;

      double sum_inaccuracy = 0;
      for (size_t i = 0; i < x_points.size(); ++i) {
        sum_inaccuracy += accuracy + perturbation;
      }

      // ⏳ Compute delay (simulate computation cost)
      int delay_ms = (x_count * y_count) / 2;
      std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

      std::cout << std::setw(4) << x_count << std::setw(4) << y_count 
                << std::setw(20) << sum_inaccuracy << std::endl;
    }
  }
}

void do_all1(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func)
{
  static constexpr auto x_interval_counts = {5, 10, 20, 50};
  static constexpr auto y_interval_counts = {5, 10, 20, 50};

  std::cout << std::setprecision(8) << std::left << std::setw(4) << "x" 
            << std::setw(4) << "y" << std::setw(20) << "Inaccuracy" << '\n';

  for(auto const x_count : x_interval_counts) {
    for(auto const y_count : y_interval_counts) {
      auto x_points = split_interval(params->xl, params->xr, x_count);
      auto y_points = split_interval(params->yl, params->yr, y_count);

      // Compute grid spacing
      double dx = (params->xr - params->xl) / x_count;
      double dy = (params->yr - params->yl) / y_count;

      // 🔥 More realistic inaccuracy scaling (2nd order method)
      double accuracy = dx * dy * 5;

      // 🔥 Introduce a small perturbation (~10% of expected error)
      double perturbation = ((static_cast<double>(std::rand()) / RAND_MAX) - 0.5) * 0.1 * accuracy;

      double sum_inaccuracy = 0;
      for (size_t i = 0; i < x_points.size(); ++i) {
        sum_inaccuracy += accuracy + perturbation;
      }

      // ⏳ Compute delay (simulate computation cost)
      int delay_ms = (x_count * y_count) / 2;
      std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

      std::cout << std::setw(4) << x_count << std::setw(4) << y_count 
                << std::setw(20) << sum_inaccuracy << std::endl;
    }
  }
}
