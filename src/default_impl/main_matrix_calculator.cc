#include <default_impl/main_matrix_calculator.hpp>

#include <cassert>

#include <contract/contract.hpp>

#include <interval_splitter.hpp>

auto DefaultMainMatrixCalculator::calc_a(Index index) const -> double
{
  // clang-format off
  contract(fun) {
    precondition(index.i != 0, "index out of range");
    precondition(index.i < m_x_points.size(), "index out of range");
    precondition(index.j < m_y_points.size(), "index out of range");
  };
  // clang-format on

  if(index.i == 0 and index.j < m_y_points.size() - 1) {                           // i == 0
    return 0;
  }
  else if(index.j == 0 and index.i < m_x_points.size() - 1) {                      // j == 0
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

auto DefaultMainMatrixCalculator::calc_b(Index index) const -> double
{
  // clang-format off
  contract(fun) {
    precondition(index.i != m_x_points.size() - 1, "index out of range");
    precondition(index.i < m_x_points.size(), "index out of range");
    precondition(index.j < m_y_points.size(), "index out of range");
  };
  // clang-format on

  auto sq = [](auto x) { return x * x; };

  if(index.i == 0 and index.j == 0) {                          // i == 0 and j == 0
    return 0;                                                  // Not too sure
  }
  else if(index.i == 0 and index.j < m_y_points.size() - 1) {  // i == 0
    return 0;
  }
  else if(index.j == 0 and index.i < m_x_points.size() - 1) {  // j == 0
    return 0;
  }
  else if(index.i == m_x_points.size() - 1 and index.j < m_y_points.size() - 1) {  // i == Nx
    return -2 * sq(calc_h(m_y_points, index.j)) / sq(calc_h(m_x_points, index.i))
         * m_input_p->k1(middle_point(m_x_points, index.i));
  }
  else if(index.i < m_x_points.size() - 1 and index.j == m_y_points.size() - 1) {  // j == Nx
    return 0;
  }
  else /*if(index.i == m_x_points.size() - 1 and index.j == m_y_points.size() - 1)*/ {
    return sq(calc_h(m_y_points, index.j)) / sq(calc_h(m_x_points, index.i))
         * m_input_p->k1(middle_point(m_x_points, index.i));
  }
}

auto DefaultMainMatrixCalculator::calc_c(Index index) const -> double
{
  // clang-format off
  contract(fun) {
    precondition(index.i < m_x_points.size(), "index out of range");
    precondition(index.j < m_y_points.size(), "index out of range");
  };
  // clang-format on

  auto sq = [](auto x) { return x * x; };

  if(index.i == 0 and index.j == 0) {                          // i == 0 and j == 0
    return 1;                                                  // Not too sure
  }
  else if(index.i == 0 and index.j < m_y_points.size() - 1) {  // i == 0
    return 1;
  }
  else if(index.j == 0 and index.i < m_x_points.size() - 1) {  // j == 0
    return 1;
  }
  else if(index.i == m_x_points.size() - 1 and index.j < m_y_points.size() - 1) {  // i == Nx
    return 2
         + sq(calc_h(m_y_points, index.j)) / sq(calc_h(m_x_points, index.i))
             * m_input_p->k1(middle_point(m_x_points, index.i))
         + 2 * sq(calc_h(m_y_points, index.j)) * m_input_p->hi2;
  }
  else if(index.i < m_x_points.size() - 1 and index.j == m_y_points.size() - 1) {  // j == Nx
    return 1;
  }
  else /*if(index.i == m_x_points.size() - 1 and index.j == m_y_points.size() - 1)*/ {
    return 2
         + sq(calc_h(m_y_points, index.j)) / sq(calc_h(m_x_points, index.i))
             * m_input_p->k1(middle_point(m_x_points, index.i))
         + sq(calc_h(m_y_points, index.j)) / sq(calc_h(m_x_points, index.i))
             * m_input_p->k1(middle_point(m_x_points, index.i));
  }
}

auto DefaultMainMatrixCalculator::calc_g(Index index) const -> double
{
  // clang-format off
  contract(fun) {
    precondition(index.i < m_x_points.size(), "index out of range");
    precondition(index.j < m_y_points.size(), "index out of range");
  };
  // clang-format on

  auto sq = [](auto x) { return x * x; };

  if(index.i == 0 and index.j == 0) {                          // i == 0 and j == 0
    return m_input_p->u1(m_y_points[index.j]);                 // Not too sure
  }
  else if(index.i == 0 and index.j < m_y_points.size() - 1) {  // i == 0
    return m_input_p->u1(m_y_points[index.j]);
  }
  else if(index.j == 0 and index.i < m_x_points.size() - 1) {  // j == 0
    return m_input_p->u3(m_x_points[index.i]);
  }
  else if(index.i == m_x_points.size() - 1 and index.j < m_y_points.size() - 1) {  // i == Nx
    return 2 * sq(calc_h(m_y_points, index.j))
           * m_input_p->f(m_x_points[index.i], m_y_points[index.j])
         + 2 * sq(calc_h(m_y_points, index.j)) * m_input_p->u2(m_y_points[index.j]);
  }
  else if(index.i < m_x_points.size() - 1 and index.j == m_y_points.size() - 1) {  // j == Nx
    return m_input_p->u4(m_x_points[index.i]);
  }
  else /*if(index.i == m_x_points.size() - 1 and index.j == m_y_points.size() - 1)*/ {
    return sq(calc_h(m_y_points, index.j)) * m_input_p->f(m_x_points[index.i], m_y_points[index.j]);
  }
}

auto DefaultMainMatrixCalculator::calc_d(Index index) const -> double
{
  // clang-format off
  contract(fun) {
    precondition(index.i < m_x_points.size(), "index out of range");
    precondition(index.j < m_y_points.size(), "index out of range");
  };
  // clang-format on

  auto sq = [](auto x) { return x * x; };

  if(index.i == 0 and index.j == 0) {                          // i == 0 and j == 0
    return 0;                                                  // Not too sure
  }
  else if(index.i == 0 and index.j < m_y_points.size() - 1) {  // i == 0
    return 0;
  }
  else if(index.j == 0 and index.i < m_x_points.size() - 1) {  // j == 0
    return 0;
  }
  else if(index.i == m_x_points.size() - 1 and index.j < m_y_points.size() - 1) {  // i == Nx
    return 0;
  }
  else if(index.i < m_x_points.size() - 1 and index.j == m_y_points.size() - 1) {  // j == Nx
    return 0;
  }
  else /*if(index.i == m_x_points.size() - 1 and index.j == m_y_points.size() - 1)*/ {
    return -sq(calc_h(m_y_points, index.j)) / sq(calc_h(m_x_points, index.i))
         * m_input_p->k1(middle_point(m_x_points, index.i + 1));
  }
}

auto DefaultMainMatrixCalculator::calc_e(Index index) const -> double
{
  // clang-format off
  contract(fun) {
    precondition(index.i < m_x_points.size(), "index out of range");
    precondition(index.j < m_y_points.size(), "index out of range");
  };
  // clang-format on

  auto sq = [](auto x) { return x * x; };

  if(index.i == 0 and index.j == 0) {                          // i == 0 and j == 0
    return 0;                                                  // Not too sure
  }
  else if(index.i == 0 and index.j < m_y_points.size() - 1) {  // i == 0
    return 0;
  }
  else if(index.j == 0 and index.i < m_x_points.size() - 1) {  // j == 0
    return 0;
  }
  else if(index.i == m_x_points.size() - 1 and index.j < m_y_points.size() - 1) {  // i == Nx
    return -1;
  }
  else if(index.i < m_x_points.size() - 1 and index.j == m_y_points.size() - 1) {  // j == Nx
    return 0;
  }
  else /*if(index.i == m_x_points.size() - 1 and index.j == m_y_points.size() - 1)*/ {
    return -1;
  }
}
