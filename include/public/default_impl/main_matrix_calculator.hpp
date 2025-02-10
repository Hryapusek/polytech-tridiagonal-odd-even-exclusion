#pragma once

#include <vector>
#include <memory>

#include <Eigen/Dense>

#include <interface/i_main_matrix_calculator.hpp>
#include <input_parameters.hpp>

class DefaultMainMatrixCalculator : public IMainMatrixCalculator
{
 public:
  explicit DefaultMainMatrixCalculator(
    std::shared_ptr<InputParameters> params,
    std::vector<double> x_points,
    std::vector<double> y_points
  )
    : m_input_p(std::move(params)),
      m_x_points(std::move(x_points)),
      m_y_points(std::move(y_points))
  {}

  auto calc_a(Index index) const -> double override;
  auto calc_b(Index index) const -> double override;
  auto calc_c(Index index) const -> double override;
  auto calc_g(Index index) const -> double override;
  auto calc_d(Index index) const -> double override;
  auto calc_e(Index index) const -> double override;

  auto params() const -> std::shared_ptr<InputParameters> const& { return m_input_p; }

  auto x_points() const -> std::vector<double> const& override { return m_x_points; }
  auto y_points() const -> std::vector<double> const& override { return m_y_points; }

 protected:
  std::shared_ptr<InputParameters> m_input_p;

  std::vector<double> m_x_points;
  std::vector<double> m_y_points;
};
