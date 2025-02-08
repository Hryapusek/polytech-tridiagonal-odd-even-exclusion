#pragma once

#include <string>

#include <defines.hpp>

#include <Eigen/Sparse>

class IBaseIntegrate
{
 public:
  virtual auto integrate(
    Eigen::VectorXd const& start_v,
    Eigen::MatrixXd const& A,
    Eigen::SparseVector<double> const& g,
    std::vector<double> const& points
  ) -> Eigen::SparseMatrix<double> = 0;

  virtual auto name() -> std::string { return "Unknown"; }
};
