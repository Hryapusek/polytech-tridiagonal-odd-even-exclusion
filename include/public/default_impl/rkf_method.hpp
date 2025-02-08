#pragma once

#include <interface/i_base_integrate.hpp>

#include <Eigen/Dense>

class RKFMethod : public IBaseIntegrate
{
 public:
  auto integrate(
    Eigen::VectorXd const& start_v,
    Eigen::MatrixXd const& A,
    Eigen::SparseVector<double> const& g,
    std::vector<double> const& points
  ) -> Eigen::SparseMatrix<double> override;

  auto name() -> std::string override { return "RKF Method"; }
};
