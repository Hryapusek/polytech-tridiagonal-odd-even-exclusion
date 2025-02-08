#pragma once

#include <functional>

// First argument is r, second is t
using R_T_Function_type = std::function<double(double, double)>;

using T_Function_type = std::function<double(double)>;
using R_Function_type = std::function<double(double)>;
