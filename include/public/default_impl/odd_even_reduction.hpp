#pragma once

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>

// Function to solve a tridiagonal system using Odd-Even Reduction
Eigen::VectorXd odd_even_reduction_solver(
  Eigen::SparseMatrix<double> const& main_matrix,
  Eigen::VectorXd const& b
);

Eigen::VectorXd odd_even_reduction_solver(
  Eigen::VectorXd const& a,
  Eigen::VectorXd const& b,
  Eigen::VectorXd const& c,
  Eigen::VectorXd const& rhs
);

Eigen::VectorXd odd_even_reduction_solver(
  Eigen::VectorXd const& a,
  Eigen::VectorXd const& b,
  Eigen::VectorXd const& c,
  Eigen::VectorXd const& d,
  Eigen::VectorXd const& e,
  Eigen::VectorXd const& rhs
);
