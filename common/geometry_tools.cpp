#include "geometry_tools.hpp"

namespace aoc {

void cramer2d(double a00, double a01, double a10, double a11, double b0,
              double b1, double &x0, double &x1) {
  x0 = (b0 * a11 - a01 * b1) / (a00 * a11 - a01 * a10);
  x1 = (a00 * b1 - b0 * a10) / (a00 * a11 - a01 * a10);
}

bool ray_intersection(double x0, double y0, double vx0, double vy0, double x1,
                      double y1, double vx1, double vy1, double &s, double &t) {
  cramer2d(vx0, -vx1, vy0, -vy1, x1 - x0, y1 - y0, s, t);
  return (s >= 0) && (t >= 0);
}

bool line_segment_intersection(double x0, double y0, double x1, double y1,
                               double x2, double y2, double x3, double y3,
                               double &s, double &t) {
  double a1 = x1 - x0;
  double b1 = -(x3 - x2);
  double a2 = y1 - y0;
  double b2 = -(y3 - y2);
  double c1 = x2 - x0;
  double c2 = y2 - y0;
  cramer2d(a1, b1, a2, b2, c1, c2, s, t);
  return (s >= 0 && s <= 1) && (t >= 0 && t <= 1);
}
} // namespace aoc
