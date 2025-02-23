#pragma once

#include <memory>

#include <defines.hpp>
#include <input_parameters.hpp>

void do_all(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func);
void do_all1(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func);
void do_all2(std::shared_ptr<InputParameters> params, X_Y_Function_type expected_func);