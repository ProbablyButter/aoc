/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
#include "geometry_tools.hpp"
#include "graph_tools.hpp"
#include "matrix.hpp"
#include "string_tools.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::vector<std::vector<char>> grid;
  std::string instrs;
  bool read_instrs = false;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    if (read_instrs) {
      instrs += line;
    } else {
      if (line.empty()) {
        read_instrs = true;
      } else {
        grid.emplace_back();
        grid.back().reserve(line.size() * 2);
        // stretch line
        for (auto c : line) {
          switch (c) {
          case '.':
            grid.back().push_back('.');
            grid.back().push_back('.');
            break;
          case '@':
            grid.back().push_back('@');
            grid.back().push_back('.');
            break;
          case '#':
            grid.back().push_back('#');
            grid.back().push_back('#');
            break;
          case 'O':
            grid.back().push_back('[');
            grid.back().push_back(']');
            break;
          }
        }
      }
    }
  }
  int pos_x = 0, pos_y = 0;
  int height = grid.size();
  int width = grid[0].size();

  std::vector<std::pair<int, int>> boxes;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      switch (grid[row][col]) {
      case '@':
        pos_x = col;
        pos_y = row;
        grid[row][col] = '.';
        break;
      case '[':
        boxes.emplace_back(row, col);
      }
    }
  }

  std::vector<std::pair<int, int>> to_move;
  int check_idx = 0;
  for (auto i : instrs) {
    int vx, vy;
    switch (i) {
    case '>':
      vx = 1;
      vy = 0;
      break;
    case '<':
      vx = -1;
      vy = 0;
      break;
    case '^':
      vx = 0;
      vy = -1;
      break;
    case 'v':
      vx = 0;
      vy = 1;
      break;
    }
    // try to move
    to_move.clear();
    check_idx = 0;
    switch (grid[pos_y + vy][pos_x + vx]) {
    case '.':
      // can move
      pos_y += vy;
      pos_x += vx;
    case '#':
      // can't move
      goto next_instr;
    case '[':
      to_move.emplace_back(pos_x + vx, pos_y + vy);
      break;
    case ']':
      to_move.emplace_back(pos_x + vx - 1, pos_y + vy);
      break;
    }
    // try to move boxes if possible
    while (true) {
      auto orig_stop = to_move.size();
      for (int j = check_idx; j < orig_stop; ++j) {
        int check_x, check_y;
        int check_x2 = -1;
        switch (i) {
        case '>':
          check_x = to_move[j].first + 2;
          check_y = to_move[j].second;
          break;
        case '<':
          check_x = to_move[j].first - 1;
          check_y = to_move[j].second;
          break;
        case '^':
          check_x = to_move[j].first;
          check_x2 = to_move[j].first + 1;
          check_y = to_move[j].second - 1;
          break;
        case 'v':
          check_x = to_move[j].first;
          check_x2 = to_move[j].first + 1;
          check_y = to_move[j].second + 1;
          break;
        }
        switch (grid[check_y][check_x]) {
        case '.':
          break;
        case '[':
          to_move.emplace_back(check_x, check_y);
          break;
        case ']':
          to_move.emplace_back(check_x - 1, check_y);
          break;
        case '#':
          goto next_instr;
        }
        if (check_x2 >= 0) {
          switch (grid[check_y][check_x2]) {
          case '.':
            break;
          case '[':
            to_move.emplace_back(check_x2, check_y);
            break;
          case ']':
            // double count if we add this one
            break;
          case '#':
            goto next_instr;
          }
        }
      }
      if (orig_stop == to_move.size()) {
        // can move
        break;
      }
      check_idx = orig_stop;
    }
    // move boxes
    // first clear boxes off grid
    for (auto &b : to_move) {
      grid[b.second][b.first] = '.';
      grid[b.second][b.first + 1] = '.';
    }
    // add new boxes
    for (auto &b : to_move) {
      grid[b.second + vy][b.first + vx] = '[';
      grid[b.second + vy][b.first + 1 + vx] = ']';
    }
    pos_x += vx;
    pos_y += vy;
  next_instr:;
  }
  int gps = 0;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      if (grid[row][col] == '[') {
        gps += row * 100 + col;
      }
    }
  }
  std::cout << gps << std::endl;
}
