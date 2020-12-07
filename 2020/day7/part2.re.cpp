/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = "char";
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
  auto YYMARKER = data.begin();
  auto YYCURSOR = data.begin();
  auto yyt1 = YYMARKER;
  auto lbl = data.end();
  std::string name;
  std::unordered_map<std::string, int> rule;
  int curr_count = 0;
parse_name:
  /*!re2c
[a-z\x20]+"bags" {
name = std::string(data.begin(), YYCURSOR-5);
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
  [0-9]
   {
curr_count = curr_count * 10 + (YYCURSOR[-1] - '0');
goto parse_list;
   }
@lbl [a-z][a-z\x20]+"bag"
{
std::string to_add(lbl, YYCURSOR - 4);
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

long long recurse_add(std::unordered_map<std::string, int> &rule,
                std::unordered_map<std::string, std::unordered_map<std::string, int>> &rules)
{
  long long count = 0;
  for (auto &v : rule)
  {
    count += v.second * (1 + recurse_add(rules[v.first], rules));
  }
  return count;
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

  // I think a recursive algorithm is the way to go?
  long long count = recurse_add(rules["shiny gold"], rules);
  std::cout << count << std::endl;
}
