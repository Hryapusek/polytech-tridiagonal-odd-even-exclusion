#pragma once

#include <vector>
#include <cstdio>
#include <span>

#include <contract/contract.hpp>

#include <defines.hpp>

auto split_interval(const double& left, const double& right, size_t num_intervals) -> std::vector<double>;

// Calculate the length of an interval `index-1` to `index`
auto calc_h(std::span<const double> intervals, size_t index) -> double;

// Calculate the cross h of an interval
auto calc_cross_h(std::span<const double> intervals, size_t index) -> double;

/// @return middle point between `index` and `index - 1`
auto middle_point(std::span<const double> intervals, size_t index) -> double;
