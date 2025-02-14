#include <interval_splitter.hpp>

auto split_interval(double const& left, double const& right, size_t num_intervals) -> std::vector<double>
{
  std::vector<double> intervals;

  contract(fun)
  {
    precondition(num_intervals > 0, "invalid number of intervals");
  };

  auto interval_size = (right - left) / num_intervals;
  for(size_t i = 0; i < num_intervals; ++i) {
    intervals.push_back(left + interval_size * i);
  }
  intervals.push_back(right);
  return intervals;
}

// Calculate the length of an interval `index-1` to `index`
auto calc_h(std::span<const double> points, size_t index) -> double
{
  contract(fun)
  {
    precondition(index < points.size(), "index out of range");
  };

  if(index == 0) {
    return points[1] - points[0];
  }

  return (points[index] - points[index - 1]);
}

// Calculate the cross h of an interval
auto calc_cross_h(std::span<const double> points, size_t index) -> double
{
  if(index == 0) {
    return calc_h(points, 1) / 2;
  }
  else if(index == points.size() - 1) {
    return calc_h(points, index) / 2;
  }
  else {
    return (calc_h(points, index) + calc_h(points, index + 1)) / 2;
  }
}

/// @return middle point between `index` and `index - 1`
auto middle_point(std::span<const double> points, size_t index) -> double
{
  return (points[index] + points[index - 1]) / 2;
}
