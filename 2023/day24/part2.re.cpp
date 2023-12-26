#include "aoc.hpp"
#include "geometry_tools.hpp"
#include "graph_tools.hpp"
#include "string_tools.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

bool collides(std::array<int64_t, 6> hail, std::array<int64_t, 3> pos,
              std::array<int64_t, 3> vel) {
  std::array<int64_t, 3> prev_hail = {hail[0], hail[1], hail[2]};
  std::array<int64_t, 3> prev_pos = pos;
  double prev_dist =
      std::hypot(prev_hail[0] - prev_pos[0], prev_hail[1] - prev_pos[1],
                 prev_hail[2] - prev_pos[2]);
  while (true) {
    for (size_t i = 0; i < 3; ++i) {
      hail[i] += hail[i + 3];
      pos[i] += vel[i];
    }
    double dist =
        std::hypot(hail[0] - pos[0], hail[1] - pos[1], hail[2] - pos[2]);
    if (dist <= 1e-2) {
      return true;
    } else {
      if (dist > prev_dist) {
        return false;
      }
      prev_dist = dist;
    }
  }
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;

  std::vector<std::array<int64_t, 6>> hail;
  std::unordered_set<std::array<int64_t, 3>, aoc::array_hasher> forbidden_vels;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    hail.emplace_back();
    auto tmp = aoc::split(line, "@");
    {
      auto tmp2 = aoc::split(tmp[0], ",");
      hail.back()[0] = aoc::to_int<int64_t>(aoc::trim(tmp2[0]));
      hail.back()[1] = aoc::to_int<int64_t>(aoc::trim(tmp2[1]));
      hail.back()[2] = aoc::to_int<int64_t>(aoc::trim(tmp2[2]));
    }
    {
      auto tmp2 = aoc::split(tmp[1], ",");
      hail.back()[3] = aoc::to_int<int64_t>(aoc::trim(tmp2[0]));
      hail.back()[4] = aoc::to_int<int64_t>(aoc::trim(tmp2[1]));
      hail.back()[5] = aoc::to_int<int64_t>(aoc::trim(tmp2[2]));
      auto div =
          std::gcd(std::gcd(std::abs(hail.back()[3]), std::abs(hail.back()[4])),
                   std::abs(hail.back()[5]));
      forbidden_vels.emplace(std::array<int64_t, 3>{
          hail.back()[3] / div, hail.back()[4] / div, hail.back()[5] / div});
      forbidden_vels.emplace(std::array<int64_t, 3>{
          -hail.back()[3] / div, -hail.back()[4] / div, -hail.back()[5] / div});
    }
  }
  int64_t max_vel = 400;
  std::vector<std::array<double, 4>> projected;
  projected.reserve(hail.size());

  std::array<int64_t, 3> pos;
  std::array<int64_t, 3> vel;

  std::vector<double> times;
  times.reserve(hail.size());

  for (int64_t vx = -max_vel; vx <= max_vel; ++vx) {
    for (int64_t vy = -max_vel; vy <= max_vel; ++vy) {
      // technically using symmetry we only need to test half of vz, but lazy
      for (int64_t vz = -max_vel; vz <= max_vel; ++vz) {
        auto gcd = std::gcd(std::gcd(std::abs(vx), std::abs(vy)), std::abs(vz));
        if (gcd != 1) {
          // a duplicate vector which points in the direction of another vector
          // we're checking
          continue;
        }
        vel[0] = vx;
        vel[1] = vy;
        vel[2] = vz;
        if (forbidden_vels.find(vel) == forbidden_vels.end()) {
          // arbitrarily chosen to not be parallel to vel, the normal
          std::array<double, 3> tangent;
          if (vz == 0) {
            if (vx == 0) {
              tangent[0] = 1;
              tangent[2] = 1;
              // solve for tangent[1]
              tangent[1] = -static_cast<double>(vx + vz) / vy;
            } else {
              tangent[1] = 1;
              tangent[2] = 1;
              // solve for tangent[0]
              tangent[0] = -static_cast<double>(vy + vz) / vx;
            }
          } else {
            // solve for tangent[2]
            tangent[0] = 1;
            tangent[1] = 1;
            tangent[2] = -static_cast<double>(vx + vy) / vz;
          }
          // B = T x N
          std::array<double, 3> binormal;
          binormal[0] = tangent[1] * vel[2] - tangent[2] * vel[1];
          binormal[1] = tangent[2] * vel[0] - tangent[0] * vel[2];
          binormal[2] = tangent[0] * vel[1] - tangent[1] * vel[0];
          double tan_mag = std::hypot(tangent[0], tangent[1], tangent[2]);
          double b_mag = std::hypot(binormal[0], binormal[1], binormal[2]);
          tangent[0] /= tan_mag;
          tangent[1] /= tan_mag;
          tangent[2] /= tan_mag;
          binormal[0] /= b_mag;
          binormal[1] /= b_mag;
          binormal[2] /= b_mag;

          projected.clear();
          times.clear();
          // project all hail onto the 2D plane normal to vel
          for (auto &h : hail) {
            projected.emplace_back();
            projected.back()[0] =
                h[0] * binormal[0] + h[1] * binormal[1] + h[2] * binormal[2];
            projected.back()[1] =
                h[0] * tangent[0] + h[1] * tangent[1] + h[2] * tangent[2];

            projected.back()[2] =
                h[3] * binormal[0] + h[4] * binormal[1] + h[5] * binormal[2];
            projected.back()[3] =
                h[3] * tangent[0] + h[4] * tangent[1] + h[5] * tangent[2];
          }
          // now find the intersection points of the projected and see if
          // they're all (roughly) the same
          double s, t;
          {
            std::array<double, 2> base_inter;
            std::array<double, 2> test_inter;
            for (size_t i = 1; i < projected.size(); ++i) {
              aoc::ray_intersection(projected[0][0], projected[0][1],
                                    projected[0][2], projected[0][3],
                                    projected[i][0], projected[i][1],
                                    projected[i][2], projected[i][3], s, t);
              if (s >= 0 && t >= 0) {
                // assume s must be an integer
                s = round(s);
                t = round(t);
                if (i == 1) {
                  times.push_back(s);
                  base_inter[0] = projected[0][0] + s * projected[0][2];
                  base_inter[1] = projected[0][1] + s * projected[0][3];
                }
                test_inter[0] = projected[0][0] + s * projected[0][2];
                test_inter[1] = projected[0][1] + s * projected[0][3];
                auto dist = std::hypot(base_inter[0] - test_inter[0],
                                       base_inter[1] - test_inter[1]);
                if (dist < 1) {
                  // TODO: check how close test_inter and base_inter are
                  times.push_back(t);
                } else {
                  break;
                }
              } else {
                break;
              }
            }
          }
          if (times.size() == hail.size()) {
            // found the velocity
            // figure out the initial position
            std::array<int64_t, 3> inter;
            inter[0] = hail[0][0] + lround(s) * hail[0][3];
            inter[1] = hail[0][1] + lround(s) * hail[0][3 + 1];
            inter[2] = hail[0][2] + lround(s) * hail[0][3 + 2];
            pos[0] = inter[0] - vx * lround(s);
            pos[1] = inter[1] - vy * lround(s);
            pos[2] = inter[2] - vz * lround(s);
            std::cout << "pos: " << pos[0] << ", " << pos[1] << ", " << pos[2]
                      << std::endl;
            std::cout << "vel: " << vx << ", " << vy << ", " << vz << std::endl;
            std::cout << pos[0] + pos[1] + pos[2] << std::endl;
            // TODO: does there exist some time which lets
            return 0;
          }
        }
      }
    }
  }
}
