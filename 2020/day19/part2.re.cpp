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
#include <map>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct rule
{
  union
  {
    int num;
    char c;
  };
  bool refers_rule;

  rule() : refers_rule(false) {}

  rule(const rule &o) : refers_rule(o.refers_rule)
  {
    if (o.refers_rule)
    {
      num = o.num;
    }
    else
    {
      c = o.c;
    }
  }

  rule &operator=(const rule &o)
  {
    refers_rule = o.refers_rule;
    if (o.refers_rule)
    {
      num = o.num;
    }
    else
    {
      c = o.c;
    }
    return *this;
  }
};

void parse_rule(const char *YYCURSOR,
                std::unordered_map<int, std::vector<std::vector<rule>>> &rules)
{
  const char *YYMARKER;

  /*!stags:re2c format = 'const char *@@;'; */
  const char *start;
  const char *stop;
  int rule_num = 0;

parse_start:
/*!re2c
@start [0-9]+ @stop ":" {
rule_num = aoc::svtoll(std::string_view(start, stop-start));
rules.emplace(rule_num, std::vector<std::vector<rule>>{});
rules[rule_num].emplace_back();
goto parse_start;
}
" " @start [0-9]+ @stop {
auto& r = rules[rule_num];
r.back().emplace_back();
r.back().back().num = aoc::svtoll(std::string_view(start, stop-start));
r.back().back().refers_rule = true;
goto parse_start;
}
"\"" @start [a-zA-Z] @stop "\"" {
auto& r = rules[rule_num];
r.back().emplace_back();
r.back().back().c = *start;
r.back().back().refers_rule = false;
goto parse_start;
}
"|" {
rules[rule_num].emplace_back();
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

void build_regex_strings(
  const std::unordered_map<int, std::vector<std::vector<rule>>> &rules,
  const std::vector<std::vector<rule>> &r,
  std::unordered_map<int, std::string> &regex_strings,
  int i)
{
  {
    auto iter = regex_strings.find(i);
    if (iter != regex_strings.end())
    {
      // already have a regex string
      return;
    }
  }
  std::stringstream res;
  // for each alternative
  for (size_t j = 0; j < r.size(); ++j)
  {
    // put each alternative into their own paren block
    res << "(";
    auto &v = r[j];
    // for each sub-rule
    for (auto &w : v)
    {
      if (w.refers_rule)
      {
        // add on the sub-rule
        auto iter = regex_strings.find(w.num);
        if (iter == regex_strings.end())
        {
          // build that regex first
          build_regex_strings(rules, rules.at(w.num), regex_strings, w.num);
          iter = regex_strings.find(w.num);
        }
        res << "(" << iter->second << ")";
      }
      else
      {
        res << w.c;
      }
    }
    res << ")";
    if (j + 1 < r.size())
    {
      res << "|";
    }
  }
  regex_strings.emplace(i, res.str());
}

int main(int argc, char **argv)
{
  using namespace std::literals;

  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;

  std::unordered_map<int, std::vector<std::vector<rule>>> rules;

  // parse rules
  while (true)
  {
    std::getline(in, line);
    if (in.eof())
    {
      break;
    }
    if (line.empty())
    {
      break;
    }
    parse_rule(line.data(), rules);
  }
  // replace rule 0, 8, and 11
  rules.erase(0);
  rules.erase(8);
  rules.erase(11);

  // turn rules into regex
  std::unordered_map<int, std::string> regex_strings;
  for (auto &v : rules)
  {
    build_regex_strings(rules, v.second, regex_strings, v.first);
  }
  // rule 0: 8 11
  // rule 8: 42 | 42 8
  // rule 11: 42 31 | 42 11 31
  // idea: figure out how many times we match rule 42 followed by 31, then make sure #42 >
  // #31 + 1

  std::regex reg42(regex_strings.at(42));
  std::regex reg31(regex_strings.at(31));

  // parse messages
  long long count = 0;
  while (true)
  {
    std::getline(in, line);
    if (in.eof())
    {
      break;
    }
    int num42 = 0;
    int num31 = 0;
    while (true)
    {
      std::smatch match_res;
      bool res =
        std::regex_search(line, match_res, reg42, std::regex_constants::match_continuous);
      if (res)
      {
        ++num42;
        line = line.substr(match_res.length(0));
      }
      else
      {
        break;
      }
    }
    while (true)
    {
      std::smatch match_res;
      bool res =
        std::regex_search(line, match_res, reg31, std::regex_constants::match_continuous);
      if (res)
      {
        ++num31;
        line = line.substr(match_res.length(0));
      }
      else
      {
        break;
      }
    }
    if (line.empty() && num31 >= 1 && num42 >= num31 + 1)
    {
      ++count;
    }
  }
  std::cout << count << std::endl;
}
