#include <default_impl/rkf_method.hpp>

#include <Eigen/Sparse>

auto RKFMethod::integrate(
  Eigen::VectorXd const& start_v,
  Eigen::MatrixXd const& A,
  Eigen::SparseVector<double> const& g,
  std::vector<double> const& points
) -> Eigen::SparseMatrix<double>
{
  auto result = Eigen::MatrixXd(A.rows(), A.cols());
  result.col(0) = start_v.sparseView();

  for(size_t i = 1; i < points.size(); ++i) {
    auto H = points.at(i) - points.at(i - 1);

    Eigen::MatrixXd u = result.col(i - 1);

    Eigen::MatrixXd k1 = (H * (A * u + g));
    Eigen::MatrixXd k2 = (H * (A * (u + 0.5 * k1) + g));
    Eigen::MatrixXd k3 = (H * (A * (u + 0.5 * k2) + g));
    Eigen::MatrixXd k4 = (H * (A * (u + k3) + g));

    result.col(i) = u + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
  }

  return result.sparseView();
}
