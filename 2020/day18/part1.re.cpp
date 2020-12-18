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
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::vector<std::string_view> parse_line(const char *YYCURSOR)
{
  const char *YYMARKER;

  /*!stags:re2c format = 'const char *@@;'; */
  std::vector<std::string_view> output_queue;
  std::vector<std::string_view> op_stack;
  const char *start;

parse_token:
  /*!re2c
@start [0-9]+ {
  output_queue.emplace_back(start, YYCURSOR-start);
  goto parse_token;
}
"(" {
  op_stack.push_back(std::string_view(YYCURSOR-1,1));
  goto parse_token;
}
")" {
  while(op_stack.size())
  {
    if(op_stack.back()[0] == '(')
    {
      op_stack.pop_back();
      break;
    }
    else
    {
      output_queue.push_back(op_stack.back());
      op_stack.pop_back();
    }
  }
  goto parse_token;
}
"+"|"*" {
  while(op_stack.size())
  {
    if (op_stack.back()[0] != '(')
    {
      output_queue.push_back(op_stack.back());
      op_stack.pop_back();
    }
    else
    {
      break;
    }
  }
  op_stack.emplace_back(YYCURSOR-1,1);
  goto parse_token;
}
[\x00] {
  goto done;
}
* {
  goto parse_token;
}
   */

done:
  while (op_stack.size())
  {
    output_queue.push_back(op_stack.back());
    op_stack.pop_back();
  }
  return output_queue;
}

long long eval_rpn(std::vector<std::string_view> &queue)
{
  long long arg;
  std::vector<long long> args;
  for (size_t i = 0; i < queue.size(); ++i)
  {
    switch (queue[i][0])
    {
    case '+':
      arg = args.back();
      args.pop_back();
      args.back() += arg;
      break;
    case '*':
      arg = args.back();
      args.pop_back();
      args.back() *= arg;
      break;
    default:
      // number
      args.push_back(aoc::svtoll(queue[i]));
      break;
    }
  }
  return args.back();
}

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;

  long long sum = 0;

  while (true)
  {
    std::getline(in, line);
    if (in.eof())
    {
      break;
    }
    std::vector<std::string_view> output_queue = parse_line(line.data());

    sum += eval_rpn(output_queue);
  }
  std::cout << sum << std::endl;
}
