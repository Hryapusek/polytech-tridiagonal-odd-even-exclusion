#pragma once

#include <interface/i_euler_explicit_method.hpp>

#include <Eigen/Dense>

class DefaultEulerExplicitMethod : public IEulerExplicitMethod
{
 public:
  auto integrate(
    Eigen::VectorXd const& start_v,
    Eigen::MatrixXd const& A,
    Eigen::SparseVector<double> const& g,
    std::vector<double> const& points
  ) -> Eigen::SparseMatrix<double> override;

  auto name() -> std::string override { return "Euler Explicit"; }
};
