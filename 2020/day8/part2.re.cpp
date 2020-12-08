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

bool try_run_code(const std::vector<std::string> &code,
                  std::vector<bool> &executed,
                  long long &acc)
{
  executed.clear();
  executed.resize(code.size(), false);
  int current_line = 0;

  const char *YYCURSOR;
  const char *YYMARKER;

  const char *start;

  /*!stags:re2c format = 'const char *@@;'; */

parse_instr:
  if (current_line >= code.size())
  {
    return true;
  }
  YYCURSOR = code[current_line].data();
  if (executed[current_line])
  {
    return false;
  }
  executed[current_line] = true;
  /*!re2c
"nop " ("+"|"-") [0-9]+ {
  ++current_line;
goto parse_instr;
}
"acc " @start ("+"|"-") [0-9]+ {
acc += atoi(start);
++current_line;
goto parse_instr;
}
"jmp " @start ("+"|"-") [0-9]+ {
current_line += atoi(start);
goto parse_instr;
}
[\x00] {
goto fail;
}
* {
goto parse_instr;
}
   */

fail:
  return false;
}

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::vector<std::string> code;
  std::vector<bool> executed;
  {
    std::string line;
    while (true)
    {
      std::getline(in, line);
      if (in.eof())
      {
        break;
      }
      code.emplace_back(std::move(line));
    }
  }
  for (auto &line : code)
  {
    long long acc = 0;
    if (line[0] == 'n')
    {
      // try to swap line with a jmp instr
      line[0] = 'j';
      line[1] = 'm';
      if (try_run_code(code, executed, acc))
      {
        std::cout << acc << std::endl;
        return 0;
      }
      line[0] = 'n';
      line[1] = 'o';
    }
    else if (line[0] == 'j')
    {
      // try to swap line with a nop instr
      line[0] = 'n';
      line[1] = 'o';
      if (try_run_code(code, executed, acc))
      {
        std::cout << acc << std::endl;
        return 0;
      }
      line[0] = 'j';
      line[1] = 'm';
    }
  }
}
