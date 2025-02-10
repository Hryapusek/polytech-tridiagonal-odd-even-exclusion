#include <default_impl/main_matrix_calculator.hpp>

#include <cassert>

#include <contract/contract.hpp>

#include <interval_splitter.hpp>

auto DefaultMainMatrixCalculator::calc_a(Index index) const -> double
{
  // clang-format off
  contract(fun) {
    precondition(index.i < m_x_points.size(), "index out of range");
    precondition(index.j < m_y_points.size(), "index out of range");
  };
  // clang-format on

  if(index.i == 0 and index.j == 0) {                          // i == 0 and j == 0
    return 1;                                                  // Not too sure
  }
  else if(index.i == 0 and index.j < m_y_points.size() - 1) {  // i == 0
    return 0;
  }
  else if(index.j == 0 and index.i < m_x_points.size() - 1) {                                      // j == 0
    return 0;
  }
  else if(index.i == m_x_points.size() - 1 and index.j < m_y_points.size() - 1) {  // i == Nx
    return -1;
  }
  else if(index.i < m_x_points.size() - 1 and index.j == m_y_points.size() - 1) {  // j == Nx
    return 0;
  }
  else /*if(index.i == m_x_points.size() - 1 and index.j == m_y_points.size() - 1)*/ {
    return 1;
  }
}

auto DefaultMainMatrixCalculator::calc_b(size_t r_index, double t) const -> double
{
  contract(fun)
  {
    precondition(
      r_index != 0,
      "You should not calculate anything for index == 0 - you already have v function"
    );
    precondition(
      r_index != r_points_.size() - 1,
      "You should not calculate b for index == intervals_.size() - 1"
    );
    precondition(r_index < r_points_.size(), "index out of range");
  }
  auto up = middle_point(r_points_, r_index + 1)
          * params_->k(middle_point(r_points_, r_index + 1), t);
  auto down = calc_h(r_points_, r_index + 1);
  return up / down;
}

/* clang-format off */
auto DefaultMainMatrixCalculator::calc_c(size_t r_index, double t) const -> double
{
  contract(fun) {
    precondition(r_index != 0, "You should not calculate anything for index == 0 - you already have v function");
    precondition(r_index < r_points_.size(), "index out of range");
  };
  if (r_index == r_points_.size() - 1) {
    return - middle_point(r_points_, r_index) * params_->k(middle_point(r_points_, r_index), t)
                 / calc_h(r_points_, r_index)
            - params_->hi2
            - params_->q(r_points_[r_index], t)
                 * calc_cross_h(r_points_, r_index);
  } else {
    auto m_point_index = middle_point(r_points_, r_index);
    auto m_point_index_plus_1 = middle_point(r_points_, r_index + 1);
    auto k_index = params_->k(m_point_index, t);
    auto k_index_plus_1 = params_->k(m_point_index_plus_1, t);
    auto h_index_plus_1 = calc_h(r_points_, r_index + 1);
    auto cross_h_index = calc_cross_h(r_points_, r_index);
    auto r = r_points_[r_index];
    auto q_index = params_->q(r_index, t);

    return -m_point_index * k_index / h_index_plus_1
           -m_point_index_plus_1 * k_index_plus_1 / h_index_plus_1
           -q_index * cross_h_index;
  }
  assert(false);
}

/* clang-format on */

/* clang-format off */
auto DefaultMainMatrixCalculator::calc_g(size_t r_index, double t) const -> double {
  contract(fun) {
    precondition(r_index != 0, "You should not calculate anything for index == 0 - you already have v function");
  };
  if (r_index == 1) {
    return params_->f(r_points_[r_index], t)
            * calc_cross_h(r_points_, r_index-1)
              +
               middle_point(r_points_, r_index) 
               * params_->k(middle_point(r_points_, r_index), t)
               / calc_h(r_points_, r_index)
               * params_->v1(t);
  } else if (r_index == r_points_.size() - 1) {
    return params_->f(r_points_[r_index], t) * calc_cross_h(r_points_, r_index)
            + params_->v2(t);
  } else {
    return params_->f(r_points_[r_index], t)  * calc_cross_h(r_points_, r_index);
  }
  assert(false);
}

/* clang-format on */
