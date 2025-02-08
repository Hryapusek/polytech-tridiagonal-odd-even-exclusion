#pragma once

#include <memory>

#include <Eigen/Dense>

#include <interface/i_main_matrix_calculator.hpp>
#include <input_parameters.hpp>

class DefaultMainMatrixCalculator : public IMainMatrixCalculator
{
 public:
  explicit DefaultMainMatrixCalculator(
    std::shared_ptr<InputParameters> params,
    std::vector<double> r_points
  )
    : params_(params)
    , r_points_(std::move(r_points))
  {}

  auto calc_a(size_t r_index, double t) const -> double override;
  auto calc_b(size_t r_index, double t) const -> double override;
  auto calc_c(size_t r_index, double t) const -> double override;
  auto calc_g(size_t r_index, double t) const -> double override;

  auto r_points() const -> std::vector<double> const& { return r_points_; }

  auto params() const -> std::shared_ptr<InputParameters> const& { return params_; }

 protected:
  std::vector<double> r_points_;
  std::shared_ptr<InputParameters> params_;
};
