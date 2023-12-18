#ifndef AOC_GEOMETRY_TOOLS_HPP
#define AOC_GEOMETRY_TOOLS_HPP

namespace aoc {
bool line_segment_intersection(double x0, double y0, double x1, double y1,
                               double x2, double y2, double x3, double y3,
                               double &s, double &t);
}

#endif
