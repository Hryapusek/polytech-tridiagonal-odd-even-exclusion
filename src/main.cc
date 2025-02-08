#include <iostream>
#include <iomanip>  // For std::setw, std::fixed, std::setprecision, etc.
#include <chrono>

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <defines.hpp>
#include <input_parameters.hpp>
#include <default_impl/main_matrix_calculator.hpp>
#include <interval_splitter.hpp>
#include <default_impl/euler_explicit_method.hpp>
#include <default_impl/euler_implicit_method.hpp>
#include <default_impl/rkf_method.hpp>

auto build_main_matrix(
  DefaultMainMatrixCalculator const& calc,
  double t,
  Eigen::MatrixXd& main_matrix
) -> Eigen::MatrixXd
{
  auto const size = calc.r_points().size() - 1;
  contract(fun) { precondition(main_matrix.rows() == size and main_matrix.cols() == size); };

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

auto integrate(
  DefaultMainMatrixCalculator const& calc,
  std::vector<double> const& r_points,
  std::vector<double> const& t_points,
  Eigen::VectorXd const& start_v,
  IBaseIntegrate& method
) -> std::tuple<Eigen::MatrixXd, double, double>
{
  auto result = Eigen::MatrixXd(Eigen::MatrixXd::Zero(r_points.size() - 1, t_points.size()));
  result.col(0) = start_v;

  std::cout << std::setprecision(4);
  Eigen::MatrixXd main_matrix(calc.r_points().size() - 1, calc.r_points().size() - 1);
  Eigen::VectorXd g;
  double total_main_matrix_time = 0;
  double total_integrate_time = 0;
  for(size_t i = 1; i < t_points.size(); ++i) {
    std::cerr << "Percentage done: " << (double)i / t_points.size() * 100 << "  ";
    std::cerr.flush();
    auto start = std::chrono::high_resolution_clock::now();

    if(dynamic_cast<IEulerImplicitMethod*>(&method)) {
      build_main_matrix(calc, t_points.at(i), main_matrix);
      g = build_g_vector(calc, t_points.at(i));
    }
    else {
      build_main_matrix(calc, t_points.at(i - 1), main_matrix);
      g = build_g_vector(calc, t_points.at(i - 1));
    }
    auto main_matrix_time = std::chrono::duration_cast<std::chrono::microseconds>(
                              std::chrono::high_resolution_clock::now() - start
    )
                              .count();
    total_main_matrix_time += main_matrix_time;
    start = std::chrono::high_resolution_clock::now();
    auto points = std::vector<double>(t_points.cbegin() + i - 1, t_points.cbegin() + i + 1);
    auto integrated_result =
      method.integrate(result.col(i - 1), main_matrix.sparseView(), g.sparseView(), points);
    auto integrate_time = std::chrono::duration_cast<std::chrono::microseconds>(
                            std::chrono::high_resolution_clock::now() - start
    )
                            .count();
    total_integrate_time += integrate_time;
    result.col(i) = integrated_result.col(1);
    std::cerr << "Time for main matrix: " << main_matrix_time
              << "us, Time for integration: " << integrate_time << "us\r";
    std::cerr.flush();
  }
  std::cerr << "\n";

  return std::make_tuple(
    result,
    total_main_matrix_time / (t_points.size() - 1),
    total_integrate_time / (t_points.size() - 1)
  );
}

void print_table_header()
{
  // Set column widths for consistent formatting
  std::cout << std::setw(20) << std::left << "Method Name" << std::setw(15) << "R Steps Count"
            << std::setw(12) << "R Step Size" << std::setw(15) << "T Steps Count" << std::setw(12)
            << "T Step Size" << std::setw(20) << "Sum Inaccuracy" << std::setw(20)
            << "Max Inaccuracy" << std::setw(30) << "Sum Inaccuracy (2nd Column)" << std::setw(30)
            << "Max Inaccuracy (2nd Column)" << std::setw(30) << "Sum Inaccuracy (Middle Col)"
            << std::setw(30) << "Max Inaccuracy (Middle Col)" << std::setw(30)
            << "Sum Inaccuracy (Last Col)" << std::setw(30) << "Max Inaccuracy (Last Col)"
            << std::setw(40) << "Main matrix build time (us)" << std::setw(40)
            << "Integration time (us)" << std::endl;

  // Print a separator line
  std::cout << std::string(260, '-') << std::endl;
}

void print_row(
  R_T_Function_type original_func,
  std::vector<double> const& r_points,
  std::vector<double> const& t_points,
  Eigen::VectorXd const& start_v,
  Eigen::MatrixXd const& result,
  IBaseIntegrate& method,
  double build_main_matrix_time,
  double integration_time
)
{
  size_t t_steps_count = t_points.size() - 1;
  size_t r_steps_count = r_points.size() - 1;

  double t_step_size = (t_points.back() - t_points.front()) / t_steps_count;
  double r_step_size = (r_points.back() - r_points.front()) / r_steps_count;

  // Initialize inaccuracies for columns and total
  double sum_inaccuracy_total = 0, max_inaccuracy_total = 0;
  double sum_inaccuracy_2nd_col = 0, max_inaccuracy_2nd_col = 0;
  double sum_inaccuracy_middle_col = 0, max_inaccuracy_middle_col = 0;
  double sum_inaccuracy_last_col = 0, max_inaccuracy_last_col = 0;
  double sum_inaccuracy_each_cell = 0;

  // Iterate over all cells of the result matrix
  for(size_t i = 1; i < r_points.size(); ++i) {  // Skip the first row (r = 0)
    for(size_t j = 0; j < t_points.size(); ++j) {
      double r = r_points[i];
      double t = t_points[j];
      double actual_value = original_func(r, t);
      double result_value = result(i - 1, j);  // Note that we offset by 1 for r = 0

      double inaccuracy = std::abs(actual_value - result_value);
      sum_inaccuracy_each_cell += inaccuracy;

      // Sum and Max inaccuracies for the whole matrix
      sum_inaccuracy_total += inaccuracy;
      max_inaccuracy_total = std::max(max_inaccuracy_total, inaccuracy);

      // Column-wise inaccuracies
      if(j == 1) {                    // Second column
        sum_inaccuracy_2nd_col += inaccuracy;
        max_inaccuracy_2nd_col = std::max(max_inaccuracy_2nd_col, inaccuracy);
      }
      if(j == t_points.size() / 2) {  // Middle column
        sum_inaccuracy_middle_col += inaccuracy;
        max_inaccuracy_middle_col = std::max(max_inaccuracy_middle_col, inaccuracy);
      }
      if(j == t_points.size() - 1) {  // Last column
        sum_inaccuracy_last_col += inaccuracy;
        max_inaccuracy_last_col = std::max(max_inaccuracy_last_col, inaccuracy);
      }
    }
  }

  // Print the row in the desired format
  std::cout << std::setw(20) << std::left << method.name()  // Method name
            << std::setw(15) << r_steps_count               // R Steps Count
            << std::setw(12) << r_step_size                 // R Step Size
            << std::setw(15) << t_steps_count               // T Steps Count
            << std::setw(12) << t_step_size                 // T Step Size
            << std::setw(20) << std::fixed << std::setprecision(6)
            << sum_inaccuracy_total                         // Sum Inaccuracy (Total)
            << std::setw(20) << max_inaccuracy_total        // Max Inaccuracy (Total)
            << std::setw(30) << sum_inaccuracy_2nd_col      // Sum Inaccuracy (2nd Column)
            << std::setw(30) << max_inaccuracy_2nd_col      // Max Inaccuracy (2nd Column)
            << std::setw(30) << sum_inaccuracy_middle_col   // Sum Inaccuracy (Middle Column)
            << std::setw(30) << max_inaccuracy_middle_col   // Max Inaccuracy (Middle Column)
            << std::setw(30) << sum_inaccuracy_last_col     // Sum Inaccuracy (Last Column)
            << std::setw(30) << max_inaccuracy_last_col     // Max Inaccuracy (Last Column)
            << std::setw(40) << build_main_matrix_time << std::setw(40) << integration_time
            << std::endl;
}

void do_all(std::shared_ptr<InputParameters> params, R_T_Function_type expected_func)
{
  print_table_header();  // Print header

  static constexpr auto t_interval_counts = {100, 500, 1'000, 2'000, 5'000, 10'000};
  static constexpr auto r_interval_counts = {5, 10, 20, 50, 100};
  static std::vector<std::shared_ptr<IBaseIntegrate>> methods =
    {std::make_shared<DefaultEulerExplicitMethod>(),
     std::make_shared<DefaultEulerImplicitMethod>(),
     std::make_shared<RKFMethod>()};

  for(auto const r_count : r_interval_counts) {
    for(auto const t_count : t_interval_counts) {
      for(auto& method : methods) {
        auto t_points = split_interval(0, params->T, t_count);
        auto r_points = split_interval(params->Rl, params->Rr, r_count);

        DefaultMainMatrixCalculator calc(params, r_points);

        Eigen::SparseVector<double> start_v(r_points.size() - 1);
        for(auto i = 0; i < r_points.size() - 1; ++i) {
          start_v.insert(i) = params->phi(r_points.at(i + 1));
        }

        auto result = integrate(calc, r_points, t_points, start_v, *method);

        print_row(
          expected_func,
          r_points,
          t_points,
          start_v,
          std::get<0>(result),
          *method,
          std::get<1>(result),
          std::get<2>(result)
        );
        std::cerr << "Finished for r: " << r_count << " t: " << t_count << "\n";
        std::cerr.flush();
        std::cout.flush();
      }
      std::cout << std::string(10, '-') << std::endl;
    }
    std::cout << std::string(50, '-') << std::endl;
  }
}

void basic_example()
{
  std::shared_ptr<InputParameters> params = std::make_shared<InputParameters>();
  params->Rl = 1;
  params->Rr = 10;
  params->T = 1;
  params->v1 = [](double t) { return 2 + t; };
  params->hi2 = 3;
  params->phi = [](double r) { return 2 * r; };
  params->v2 = [](double t) { return 62 + 3 * t; };

  params->k = [](double r, double t) { return 1.0; };
  params->q = [](double r, double t) { return 3.0; };
  params->f = [](double r, double t) { return 3 * t + 6 * r + 1 - 2 / r; };

  auto expected_func = [](double r, double t) { return t + 2 * r; };

  do_all(params, expected_func);
}

void hard_example()
{
  std::shared_ptr<InputParameters> params = std::make_shared<InputParameters>();
  params->Rl = 1;
  params->Rr = 10;
  params->T = 1;
  params->v1 = [](double t) { return 5 + t * t; };
  params->hi2 = 5;
  params->phi = [](double r) { return 5 * r * r; };
  params->v2 = [](double t) { return 5*(500 + t * t) + (20 + t*t)*(100 + t * t); };

  params->k = [](double r, double t) { return 2 * r + t * t; };
  params->q = [](double r, double t) { return 3 * r * r + t; };
  params->f = [](double r, double t) {
    return 2 * t - 40 - 20 * t * t + 15 * r * r * r * r + 3 * r * r * t * t + 5 * r * r * t
         + t * t * t;
  };

  auto expected_func = [](double r, double t) { return 5 * r * r + t * t; };

  do_all(params, expected_func);
}

int main()
{
  // basic_example();
  hard_example();
  return 0;
}
