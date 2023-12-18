#include "geometry_tools.hpp"

namespace aoc {

bool line_segment_intersection(double x0, double y0, double x1, double y1,
                               double x2, double y2, double x3, double y3,
                               double &s, double &t) {
  double a1 = x1 - x0;
  double b1 = -(x3 - x2);
  double a2 = y1 - y0;
  double b2 = -(y3 - y2);
  double c1 = x2 - x0;
  double c2 = y2 - y0;
  s = (c1 * b2 - b1 * c2) / (a1 * b2 - b1 * a2);
  t = (a1 * c2 - c1 * a2) / (a1 * b2 - b1 * a2);
  return (s >= 0 && s <= 1) && (t >= 0 && t <= 1);
}
} // namespace aoc
