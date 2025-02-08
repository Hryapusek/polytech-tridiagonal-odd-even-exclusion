#pragma once

#include <interface/i_euler_implicit_method.hpp>

#include <Eigen/Dense>

class DefaultEulerImplicitMethod : public IEulerImplicitMethod
{
 public:
  auto integrate(
    Eigen::VectorXd const& start_v,
    Eigen::MatrixXd const& A,
    Eigen::SparseVector<double> const& g,
    std::vector<double> const& points
  ) -> Eigen::SparseMatrix<double> override;

  auto name() -> std::string override { return "Euler Implicit"; }
};
