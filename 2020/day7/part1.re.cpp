/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

void parse_rule(const std::string &data,
                std::unordered_map<std::string, std::unordered_map<std::string, int>> &rules)
{
  const char* YYCURSOR = data.data();
  const char* YYMARKER;
  std::string name;
  std::unordered_map<std::string, int> rule;
  int curr_count = 0;
  const char *start, *stop;

  /*!stags:re2c format = 'const char *@@;'; */
parse_name:
  /*!re2c
@start [a-z][a-z\x20]+[a-z] @stop " bags" {
name = std::string(start, stop);
goto parse_list;
}
[\x00] {
return;
}
* {
goto parse_name;
}
   */

parse_list:
  /*!re2c
  @start [0-9]
   {
curr_count = curr_count * 10 + (*start - '0');
goto parse_list;
   }
@start [a-z][a-z\x20]+[a-z] @stop " bag"
{
std::string to_add(start, stop);
rule.emplace(std::move(to_add), curr_count);
curr_count = 0;
goto parse_list;
}
"." {
goto done;
}
[\x00] {
goto done;
}
   * {
goto parse_list;
}
  */

done:
  rules.emplace(name, std::move(rule));
}

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);

  std::string line;
  std::unordered_map<std::string, std::unordered_map<std::string, int>> rules;

  // parse the input
  while (true)
  {
    std::getline(in, line);
    if (in.eof())
    {
      break;
    }
    parse_rule(line, rules);
  }

  // maps a bag to all other bag types which can directly hold it
  std::unordered_map<std::string, std::vector<std::string>> inverse_rules;
  for (auto &rule : rules)
  {
    for (auto &b : rule.second)
    {
      auto iter = inverse_rules.emplace(b.first, std::vector<std::string>());
      iter.first->second.emplace_back(rule.first);
    }
  }

  std::unordered_set<std::string> can_hold;
  for (auto &v : inverse_rules["shiny gold"])
  {
    can_hold.emplace(v);
  }

  bool any_added = false;
  do
  {
    std::vector<std::string> to_add;
    for (auto &v : can_hold)
    {
      for (auto &w : inverse_rules[v])
      {
        to_add.emplace_back(w);
      }
    }
    auto prev_size = can_hold.size();
    for (auto &v : to_add)
    {
      can_hold.emplace(v);
    }
    any_added = (can_hold.size() > prev_size);
  } while (any_added);
  std::cout << can_hold.size() << std::endl;
}
