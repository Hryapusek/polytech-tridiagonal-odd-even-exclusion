#pragma once

#include <defines.hpp>

struct InputParameters {
  double Rl;
  double Rr; // [Rl, Rr]
  double T; // [0, T]

  // First type condition
  T_Function_type v1; // u(rL) = v1(t)

  // Third type condition
  double hi2; // -k * du/dr = hi2 * u(rR) - v2(t)
  R_Function_type phi;
  T_Function_type v2;

  // Just input functions
  R_T_Function_type k;
  R_T_Function_type q;
  R_T_Function_type f;
};
