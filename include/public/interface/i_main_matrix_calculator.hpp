#pragma once

#include <cstdio>

class IMainMatrixCalculator
{
 public:
  virtual auto calc_a(size_t r_index, double t) const -> double = 0;
  virtual auto calc_b(size_t r_index, double t) const -> double = 0;
  virtual auto calc_c(size_t r_index, double t) const -> double = 0;
  virtual auto calc_g(size_t r_index, double t) const -> double = 0;
};
