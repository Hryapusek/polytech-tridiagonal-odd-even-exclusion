#include <default_impl/main_matrix_calculator.hpp>

#include <cassert>

#include <contract/contract.hpp>

#include <interval_splitter.hpp>

auto DefaultMainMatrixCalculator::calc_a(Index index) const -> double
{
  contract(fun) {
    precondition(index.i > 0, "index out of range");  // Prevent accessing invalid left neighbor
  };

  double dx = calc_h(interiour_x_points(), index.i);  
  double k_left = m_input_p->k1(middle_point(interiour_x_points(), index.i - 1));  // Use i-1

  return - k_left / (dx * dx);
}

auto DefaultMainMatrixCalculator::calc_b(Index index) const -> double
{
  contract(fun) {
    precondition(index.i < interiour_x_points().size() - 1, "index out of range");
  };

  double dx = calc_h(interiour_x_points(), index.i);  // Use index.i instead of index.i + 1
  double k_right = m_input_p->k1(middle_point(interiour_x_points(), index.i + 1));

  return - k_right / (dx * dx);
}

auto DefaultMainMatrixCalculator::calc_c(Index index) const -> double
{
  contract(fun) {
    // precondition(index.i < interiour_x_points().size() - 1, "index out of range");  // Fix boundary condition
    precondition(index.j < interiour_y_points().size(), "index out of range");
  };

  double dx = calc_h(interiour_x_points(), index.i);
  double dy = calc_h(interiour_y_points(), index.j);

  double k_left = m_input_p->k1(middle_point(interiour_x_points(), index.i - 1));  
  double k_right = m_input_p->k1(middle_point(interiour_x_points(), index.i + 1));

  return (k_right + k_left) / (dx * dx) + 2.0 / (dy * dy);
}

auto DefaultMainMatrixCalculator::calc_d(Index index) const -> double
{
  contract(fun) {
    precondition(index.j > 0, "index out of range");
  };

  double dy = calc_h(interiour_y_points(), index.j);

  return - 1.0 / (dy * dy);
}

auto DefaultMainMatrixCalculator::calc_e(Index index) const -> double
{
  contract(fun) {
    precondition(index.j < interiour_y_points().size() - 1, "index out of range");  // Add check
  };

  double dy = calc_h(interiour_y_points(), index.j);

  return - 1.0 / (dy * dy);
}

auto DefaultMainMatrixCalculator::calc_g(Index index) const -> double
{
  contract(fun) {
    precondition(index.i < interiour_x_points().size(), "index out of range");
    precondition(index.j < interiour_y_points().size(), "index out of range");
  };

  double dx = calc_h(interiour_x_points(), index.i);
  double dy = calc_h(interiour_y_points(), index.j);

  if (index.i == 0) {  // Dirichlet at x = a
    return m_input_p->u1(interiour_y_points()[index.j]);
  }
  else if (index.j == 0) {  // Dirichlet at y = c
    return m_input_p->u3(interiour_x_points()[index.i]);
  }
  else if (index.j == interiour_y_points().size() - 1) {  // Dirichlet at y = d
    return m_input_p->u4(interiour_x_points()[index.i]);
  }
  else if (index.i == interiour_x_points().size() - 1) {  // Robin at x = b
    return (2.0 / dx) * (m_input_p->f(interiour_x_points()[index.i], interiour_y_points()[index.j])
                        + m_input_p->u2(interiour_y_points()[index.j]));
  }
  else {  // Interior points
    return dx * dy * m_input_p->f(interiour_x_points()[index.i], interiour_y_points()[index.j]);
  }
}

// auto DefaultMainMatrixCalculator::calc_a(Index index) const -> double
// {
//   // clang-format off
//   contract(fun) {
//     precondition(index.i != 0, "index out of range");
//     precondition(index.i < m_x_points.size(), "index out of range");
//     precondition(index.j < m_y_points.size(), "index out of range");
//   };
//   // clang-format on

//   if(index.i == 0 and index.j < m_y_points.size() - 1) {                           // i == 0
//     return 0;
//   }
//   else if(index.j == 0 and index.i < m_x_points.size() - 1) {                      // j == 0
//     return 0;
//   }
//   else if(index.i == m_x_points.size() - 1 and index.j < m_y_points.size() - 1) {  // i == Nx
//     return -1;
//   }
//   else if(index.i < m_x_points.size() - 1 and index.j == m_y_points.size() - 1) {  // j == Nx
//     return 0;
//   }
//   else /*if(index.i == m_x_points.size() - 1 and index.j == m_y_points.size() - 1)*/ {
//     return 1;
//   }
// }

// auto DefaultMainMatrixCalculator::calc_b(Index index) const -> double
// {
//   // clang-format off
//   contract(fun) {
//     precondition(index.i != m_x_points.size() - 1, "index out of range");
//     precondition(index.i < m_x_points.size(), "index out of range");
//     precondition(index.j < m_y_points.size(), "index out of range");
//   };
//   // clang-format on

//   auto sq = [](auto x) { return x * x; };

//   if(index.i == 0 and index.j == 0) {                          // i == 0 and j == 0
//     return 0;                                                  // Not too sure
//   }
//   else if(index.i == 0 and index.j < m_y_points.size() - 1) {  // i == 0
//     return 0;
//   }
//   else if(index.j == 0 and index.i < m_x_points.size() - 1) {  // j == 0
//     return 0;
//   }
//   else if(index.i == m_x_points.size() - 1 and index.j < m_y_points.size() - 1) {  // i == Nx
//     return -2 * sq(calc_h(m_y_points, index.j)) / sq(calc_h(m_x_points, index.i))
//          * m_input_p->k1(middle_point(m_x_points, index.i));
//   }
//   else if(index.i < m_x_points.size() - 1 and index.j == m_y_points.size() - 1) {  // j == Nx
//     return 0;
//   }
//   else /*if(index.i == m_x_points.size() - 1 and index.j == m_y_points.size() - 1)*/ {
//     return sq(calc_h(m_y_points, index.j)) / sq(calc_h(m_x_points, index.i))
//          * m_input_p->k1(middle_point(m_x_points, index.i));
//   }
// }

// auto DefaultMainMatrixCalculator::calc_c(Index index) const -> double
// {
//   // clang-format off
//   contract(fun) {
//     precondition(index.i < m_x_points.size(), "index out of range");
//     precondition(index.j < m_y_points.size(), "index out of range");
//   };
//   // clang-format on

//   auto sq = [](auto x) { return x * x; };

//   if(index.i == 0 and index.j == 0) {                          // i == 0 and j == 0
//     return 1;                                                  // Not too sure
//   }
//   else if(index.i == 0 and index.j < m_y_points.size() - 1) {  // i == 0
//     return 1;
//   }
//   else if(index.j == 0 and index.i < m_x_points.size() - 1) {  // j == 0
//     return 1;
//   }
//   else if(index.i == m_x_points.size() - 1 and index.j < m_y_points.size() - 1) {  // i == Nx
//     return 2
//          + sq(calc_h(m_y_points, index.j)) / sq(calc_h(m_x_points, index.i))
//              * m_input_p->k1(middle_point(m_x_points, index.i))
//          + 2 * sq(calc_h(m_y_points, index.j)) * m_input_p->hi2;
//   }
//   else if(index.i < m_x_points.size() - 1 and index.j == m_y_points.size() - 1) {  // j == Nx
//     return 1;
//   }
//   else /*if(index.i == m_x_points.size() - 1 and index.j == m_y_points.size() - 1)*/ {
//     return 2
//          + sq(calc_h(m_y_points, index.j)) / sq(calc_h(m_x_points, index.i))
//              * m_input_p->k1(middle_point(m_x_points, index.i))
//          + sq(calc_h(m_y_points, index.j)) / sq(calc_h(m_x_points, index.i))
//              * m_input_p->k1(middle_point(m_x_points, index.i));
//   }
// }

// auto DefaultMainMatrixCalculator::calc_g(Index index) const -> double
// {
//   // clang-format off
//   contract(fun) {
//     precondition(index.i < m_x_points.size(), "index out of range");
//     precondition(index.j < m_y_points.size(), "index out of range");
//   };
//   // clang-format on

//   auto sq = [](auto x) { return x * x; };

//   if(index.i == 0 and index.j == 0) {                          // i == 0 and j == 0
//     return m_input_p->u1(m_y_points[index.j]);                 // Not too sure
//   }
//   else if(index.i == 0 and index.j < m_y_points.size() - 1) {  // i == 0
//     return m_input_p->u1(m_y_points[index.j]);
//   }
//   else if(index.j == 0 and index.i < m_x_points.size() - 1) {  // j == 0
//     return m_input_p->u3(m_x_points[index.i]);
//   }
//   else if(index.i == m_x_points.size() - 1 and index.j < m_y_points.size() - 1) {  // i == Nx
//     return 2 * sq(calc_h(m_y_points, index.j))
//            * m_input_p->f(m_x_points[index.i], m_y_points[index.j])
//          + 2 * sq(calc_h(m_y_points, index.j)) * m_input_p->u2(m_y_points[index.j]);
//   }
//   else if(index.i < m_x_points.size() - 1 and index.j == m_y_points.size() - 1) {  // j == Nx
//     return m_input_p->u4(m_x_points[index.i]);
//   }
//   else /*if(index.i == m_x_points.size() - 1 and index.j == m_y_points.size() - 1)*/ {
//     return sq(calc_h(m_y_points, index.j)) * m_input_p->f(m_x_points[index.i], m_y_points[index.j]);
//   }
// }

// auto DefaultMainMatrixCalculator::calc_d(Index index) const -> double
// {
//   // clang-format off
//   contract(fun) {
//     precondition(index.i < m_x_points.size(), "index out of range");
//     precondition(index.j < m_y_points.size(), "index out of range");
//   };
//   // clang-format on

//   auto sq = [](auto x) { return x * x; };

//   if(index.i == 0 and index.j == 0) {                          // i == 0 and j == 0
//     return 0;                                                  // Not too sure
//   }
//   else if(index.i == 0 and index.j < m_y_points.size() - 1) {  // i == 0
//     return 0;
//   }
//   else if(index.j == 0 and index.i < m_x_points.size() - 1) {  // j == 0
//     return 0;
//   }
//   else if(index.i == m_x_points.size() - 1 and index.j < m_y_points.size() - 1) {  // i == Nx
//     return 0;
//   }
//   else if(index.i < m_x_points.size() - 1 and index.j == m_y_points.size() - 1) {  // j == Nx
//     return 0;
//   }
//   else /*if(index.i == m_x_points.size() - 1 and index.j == m_y_points.size() - 1)*/ {
//     return -sq(calc_h(m_y_points, index.j)) / sq(calc_h(m_x_points, index.i))
//          * m_input_p->k1(middle_point(m_x_points, index.i + 1));
//   }
// }

// auto DefaultMainMatrixCalculator::calc_e(Index index) const -> double
// {
//   // clang-format off
//   contract(fun) {
//     precondition(index.i < m_x_points.size(), "index out of range");
//     precondition(index.j < m_y_points.size(), "index out of range");
//   };
//   // clang-format on

//   auto sq = [](auto x) { return x * x; };

//   if(index.i == 0 and index.j == 0) {                          // i == 0 and j == 0
//     return 0;                                                  // Not too sure
//   }
//   else if(index.i == 0 and index.j < m_y_points.size() - 1) {  // i == 0
//     return 0;
//   }
//   else if(index.j == 0 and index.i < m_x_points.size() - 1) {  // j == 0
//     return 0;
//   }
//   else if(index.i == m_x_points.size() - 1 and index.j < m_y_points.size() - 1) {  // i == Nx
//     return -1;
//   }
//   else if(index.i < m_x_points.size() - 1 and index.j == m_y_points.size() - 1) {  // j == Nx
//     return 0;
//   }
//   else /*if(index.i == m_x_points.size() - 1 and index.j == m_y_points.size() - 1)*/ {
//     return -1;
//   }
// }
