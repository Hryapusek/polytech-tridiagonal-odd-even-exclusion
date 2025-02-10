#pragma once

#include <functional>

// First argument is X, second is Y
using X_Y_Function_type = std::function<double(double, double)>;

using X_Function_type = std::function<double(double)>;
using Y_Function_type = std::function<double(double)>;
