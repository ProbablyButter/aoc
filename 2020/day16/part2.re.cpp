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

void parse_rule(const char *YYCURSOR,
                std::vector<std::array<int, 4>> &rules,
                std::vector<std::string> &rule_names)
{
  std::array<int, 4> res = {0, 0, 0, 0};
  const char *YYMARKER;
  const char *start;
  const char *stop;
  const char *m0;
  const char *m1;
  const char *m2;
  const char *m3;
  const char *m4;
  const char *m5;
  const char *m6;

  /*!stags:re2c format = 'decltype(YYCURSOR) @@;'; */
parse_start:
  /*!re2c
@start [a-z\x20]+ @stop ": " @m0 [0-9]+ @m1 "-" @m2 [0-9]+ @m3 " or " @m4 [0-9]+ @m5 "-"
@m6 [0-9]+ { rule_names.emplace_back(std::string_view(start, stop-start)); res[0] =
aoc::svtoll(std::string_view(m0, m1-start)); res[1] = aoc::svtoll(std::string_view(m2,
m3-m2)); res[2] = aoc::svtoll(std::string_view(m4, m5-m4)); res[3] =
aoc::svtoll(std::string_view(m6, YYCURSOR-m6)); rules.push_back(res); goto done;
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
  std::vector<std::string> rule_names;
  std::vector<int> to_mult;
  std::string departure = "departure";
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

      parse_rule(line.data(), rules, rule_names);
      if (rule_names.back().size() >= departure.size() &&
          rule_names.back().substr(0, departure.size()) == departure)
      {
        to_mult.push_back(rule_names.size() - 1);
      }
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
  // remove bad tickets
  std::vector<int> invalid_values;
  for (size_t i = 0; i < other_tickets.size();)
  {
    invalid_values.clear();
    invalid_ticket(other_tickets[i], rules, invalid_values);
    if (!invalid_values.empty())
    {
      other_tickets.erase(other_tickets.begin() + i);
    }
    else
    {
      ++i;
    }
  }
  std::cout << other_tickets.size() << " valid other tickets" << std::endl;
  other_tickets.push_back(my_ticket);

  // try to figure out which fields are which
  // maps a rule -> what index in ticket it is
  std::vector<std::vector<int>> valid_cols;
  valid_cols.resize(rules.size());
  for (size_t ridx = 0; ridx < rules.size(); ++ridx)
  {
    for (size_t idx = 0; idx < rules.size(); ++idx)
    {
      // figure out if a given column matches rules[ridx]
      bool valid = true;
      for (auto &ticket : other_tickets)
      {
        if (!((ticket[idx] >= rules[ridx][0] && ticket[idx] <= rules[ridx][1]) ||
              (ticket[idx] >= rules[ridx][2] && ticket[idx] <= rules[ridx][3])))
        {
          valid = false;
          break;
        }
      }
      if (valid)
      {
        valid_cols[ridx].push_back(idx);
      }
    }
  }
  // now need to keep logically removing columns from rules until all rules have 1 valid
  // col it maps to
  while (true)
  {
    size_t num_done = 0;
    for (size_t i = 0; i < valid_cols.size(); ++i)
    {
      if (valid_cols[i].size() == 1)
      {
        ++num_done;
        // remove entries in other rules
        for (size_t j = 0; j < valid_cols.size(); ++j)
        {
          if (i != j)
          {
            valid_cols[j].erase(
              std::remove(valid_cols[j].begin(), valid_cols[j].end(), valid_cols[i][0]),
              valid_cols[j].end());
          }
        }
      }
    }
    if (num_done >= valid_cols.size())
    {
      break;
    }
  }
  
  for (size_t i = 0; i < valid_cols.size(); ++i)
  {
    for(size_t j = 0; j < valid_cols[i].size(); ++j)
    {
      std::cout << rule_names[i] << ": " << valid_cols[i][j] << ", ";
    }
    std::cout << std::endl;
  }

  long res = 1;
  for (size_t i = 0; i < to_mult.size(); ++i)
  {
    res *= my_ticket[valid_cols[to_mult[i]][0]];
  }
  std::cout << res << std::endl;
}
