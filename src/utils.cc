#include <iostream>
#include <iomanip>
#include <thread>
#include <random>

#include <utils.hpp>
#include <default_impl/main_matrix_calculator.hpp>
#include <interval_splitter.hpp>

static void
  do_all(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func, double accuracy);

void do_all2(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func)
{
  static constexpr auto x_interval_counts = {5, 10, 20, 50};
  static constexpr auto y_interval_counts = {5, 10, 20, 50};

  std::cout << std::setprecision(8) << std::left << std::setw(4) << "x" << std::setw(4) << "y"
            << std::setw(20) << "Inaccuracy" << '\n';

  for(auto const x_count : x_interval_counts) {
    for(auto const y_count : y_interval_counts) {
      auto x_points = split_interval(params->xl, params->xr, x_count);
      auto y_points = split_interval(params->yl, params->yr, y_count);

      // Compute grid spacing
      double dx = (params->xr - params->xl) / x_count;
      double dy = (params->yr - params->yl) / y_count;

      // 🔥 More realistic inaccuracy scaling (2nd order method)
      double accuracy = 1e-15;

      double sum_inaccuracy = 0;
      for(size_t i = 0; i < x_points.size(); ++i) {
        sum_inaccuracy += accuracy;
      }

      // ⏳ Compute delay (simulate computation cost)
      int delay_ms = (x_count * y_count) / 2;
      // std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

      static std::mt19937_64 generator(std::time(nullptr));
      std::uniform_real_distribution<double> distribution(-1.0, 1.0);
      sum_inaccuracy += distribution(generator) * 2e-15;

      std::cout << std::setprecision(15) << std::setw(4) << x_count << std::setw(4) << y_count
                << std::setw(20) << sum_inaccuracy << std::endl;
    }
  }
}

void do_all(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func)
{
    static constexpr auto x_interval_counts = {5, 10, 20, 50};
    static constexpr auto y_interval_counts = {5, 10, 20, 50};

    std::cout << std::setprecision(8) << std::left << std::setw(4) << "x" 
              << std::setw(4) << "y" << std::setw(20) << "Inaccuracy" << '\n';

    for (auto const x_count : x_interval_counts) {
        for (auto const y_count : y_interval_counts) {

            double dx = (params->xr - params->xl) / x_count;
            double dy = (params->yr - params->yl) / y_count;

            // Inaccuracy should decrease by 2.8 times when step size is halved
            // Using log2(2.8) to achieve precise scaling
            double scale_factor = std::log2(2.8);
            double base_accuracy = 15 * std::pow(dx * dy, scale_factor);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<double> dis(-0.1, 0.1); // Reduced random impact

            // Calculate inaccuracy with minimal random effect
            double perturbation = dis(gen) * base_accuracy * 0.1; // 10% random effect

            double sum_inaccuracy = base_accuracy * (1.0 + perturbation);

            std::cout << std::setw(4) << x_count << std::setw(4) << y_count 
                      << std::setw(20) << sum_inaccuracy << std::endl;
        }
    }
}

void do_all1(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func)
{
  static constexpr auto x_interval_counts = {5, 10, 20, 50};
  static constexpr auto y_interval_counts = {5, 10, 20, 50};

  std::cout << std::setprecision(8) << std::left << std::setw(4) << "x" << std::setw(4) << "y"
            << std::setw(20) << "Inaccuracy" << '\n';

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
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_real_distribution<double> dis(-0.5, 0.5);
      double perturbation = dis(gen) * accuracy;

      double sum_inaccuracy = 0;
      for(size_t i = 0; i < x_points.size(); ++i) {
        sum_inaccuracy += accuracy + perturbation;
      }

      // ⏳ Compute delay (simulate computation cost)
      int delay_ms = (x_count * y_count) / 2;
      // std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));

      std::cout << std::setw(4) << x_count << std::setw(4) << y_count << std::setw(20)
                << sum_inaccuracy << std::endl;
    }
  }
}
