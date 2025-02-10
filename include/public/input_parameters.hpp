#pragma once

#include <defines.hpp>

struct InputParameters {
  double xl;
  double xr;
  double yl;
  double yr;

  // First type condition
  Y_Function_type u1;

  // Third type condition
  double hi2;
  X_Function_type k1;
  Y_Function_type u2;

  // First type condition
  X_Function_type u3;

  // First type condition
  X_Function_type u4;

  // Just input functions
  X_Y_Function_type f;
};
