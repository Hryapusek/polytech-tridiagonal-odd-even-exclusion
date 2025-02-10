#pragma once

#include <Eigen/Dense>
#include <vector>

// Function to solve a tridiagonal system using Odd-Even Reduction
Eigen::VectorXd odd_even_reduction_solver(
  Eigen::MatrixXd const& main_matrix,
  Eigen::VectorXd const& b
);

Eigen::VectorXd odd_even_reduction_solver(
  Eigen::VectorXd const& a,
  Eigen::VectorXd const& b,
  Eigen::VectorXd const& c,
  Eigen::VectorXd const& rhs
);
