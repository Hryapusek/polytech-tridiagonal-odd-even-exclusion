#pragma once

#include <cstdio>

struct Index
{
  size_t i = -1;
  size_t j = -1;
};

class IMainMatrixCalculator
{
 public:
  virtual auto calc_a(Index index) const -> double = 0;
  virtual auto calc_b(Index index) const -> double = 0;
  virtual auto calc_c(Index index) const -> double = 0;
  virtual auto calc_d(Index index) const -> double = 0;
  virtual auto calc_e(Index index) const -> double = 0;
  virtual auto calc_g(Index index) const -> double = 0;

  virtual auto x_points() const -> std::vector<double> const& = 0;
  virtual auto y_points() const -> std::vector<double> const& = 0;
};
