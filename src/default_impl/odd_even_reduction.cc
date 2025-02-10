#include <default_impl/odd_even_reduction.hpp>

Eigen::VectorXd odd_even_reduction_solver(
  Eigen::VectorXd const& a,
  Eigen::VectorXd const& b,
  Eigen::VectorXd const& c,
  Eigen::VectorXd const& rhs
)
{
  int n = rhs.size();

  if(n == 1) {
    return rhs.array() / b.array();
  }

  int n_half = n / 2;
  Eigen::VectorXd a_half(n_half), b_half(n_half), c_half(n_half), rhs_half(n_half);

  for(int i = 0; i < n_half; ++i) {
    int j = 2 * i + 1;
    double denom = b[j] - a[j] * c[j - 1] / b[j - 1];

    b_half[i] = denom;
    rhs_half[i] = rhs[j] - a[j] * rhs[j - 1] / b[j - 1];

    if(j + 1 < n) {
      a_half[i] = -a[j + 1];
      c_half[i] = -c[j - 1] * c[j] / b[j];
    }
  }

  Eigen::VectorXd x_half = odd_even_reduction_solver(a_half, b_half, c_half, rhs_half);

  Eigen::VectorXd x(n);
  for(int i = 0; i < n_half; ++i) {
    x[2 * i + 1] = x_half[i];
  }

  for(int i = 0; i < n_half; ++i) {
    int j = 2 * i;
    x[j] = (rhs[j] - c[j] * x[j + 1]) / b[j];
  }

  return x;
}

Eigen::VectorXd
  odd_even_reduction_solver(Eigen::MatrixXd const& main_matrix, Eigen::VectorXd const& b)
{
  return Eigen::VectorXd::Zero(b.size());
}
