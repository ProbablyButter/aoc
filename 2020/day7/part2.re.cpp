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
