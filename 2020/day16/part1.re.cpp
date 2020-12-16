/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::array<int, 4> parse_rule(const char *YYCURSOR)
{
  std::array<int, 4> res = {0, 0, 0, 0};
  const char *YYMARKER;
  const char *start;
  const char *m1;
  const char *m2;
  const char *m3;
  const char *m4;
  const char *m5;
  const char *m6;

  /*!stags:re2c format = 'decltype(YYCURSOR) @@;'; */
parse_start:
  /*!re2c
@start [0-9]+ @m1 "-" @m2 [0-9]+ @m3 " or " @m4 [0-9]+ @m5 "-" @m6 [0-9]+ {
res[0] = aoc::svtoll(std::string_view(start, m1-start));
res[1] = aoc::svtoll(std::string_view(m2, m3-m2));
res[2] = aoc::svtoll(std::string_view(m4, m5-m4));
res[3] = aoc::svtoll(std::string_view(m6, YYCURSOR-m6));
goto done;
}
[\x00] {
goto done;
}
* {
goto parse_start;
}
   */
done:
  return res;
}

void parse_ticket(const char *YYCURSOR, std::vector<int> &ticket)
{

  const char *YYMARKER;
  const char *start;

  /*!stags:re2c format = 'decltype(YYCURSOR) @@;'; */
parse_start:
  /*!re2c
@start [0-9]+ {
int v = aoc::svtoll(std::string_view(start, YYCURSOR-start));
ticket.push_back(v);
goto parse_start;
}
[\x00] {
goto done;
}
* {
goto parse_start;
}
   */
done:
  return;
}

void invalid_ticket(const std::vector<int> &ticket,
                    std::vector<std::array<int, 4>> &rules,
                    std::vector<int> &invalid_vals)
{
  for (auto v : ticket)
  {
    bool valid = false;
    // does v match any rules?
    for (auto &rule : rules)
    {
      if ((v >= rule[0] && v <= rule[1]) || (v >= rule[2] && v <= rule[3]))
      {
        valid = true;
        break;
      }
    }
    if (!valid)
    {
      invalid_vals.push_back(v);
    }
  }
}

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;
  std::vector<std::array<int, 4>> rules;
  // parse rules
  while (true)
  {
    std::getline(in, line);
    if (in.eof())
    {
      break;
    }
    if (!line.empty())
    {
      // start of "your ticket:"
      if (line[0] == 'y')
      {
        break;
      }

      rules.push_back(parse_rule(line.data()));
    }
  }
  // parse your ticket
  std::vector<int> my_ticket;
  // "your ticket:"
  std::getline(in, line);
  parse_ticket(line.data(), my_ticket);
  // blank line
  std::getline(in, line);
  // "nearby tickets:"
  std::getline(in, line);
  // parse everyone else's ticket
  std::vector<std::vector<int>> other_tickets;
  while (true)
  {
    std::getline(in, line);
    if (in.eof())
    {
      break;
    }
    other_tickets.emplace_back();
    parse_ticket(line.data(), other_tickets.back());
  }
  // time to do challenge
  std::vector<int> invalid_values;
  int count = 0;
  for (size_t i = 0; i < other_tickets.size(); ++i)
  {
    auto v = invalid_values.size();
    invalid_ticket(other_tickets[i], rules, invalid_values);
    if (v == invalid_values.size() && !other_tickets[i].empty())
    {
      ++count;
    }
  }
  std::cout << count << " valid other tickets" << std::endl;
  int sum = 0;
  for (auto v : invalid_values)
  {
    sum += v;
  }
  std::cout << sum << std::endl;
}
