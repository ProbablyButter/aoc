/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = "char";
re2c:yyfill:enable = 0;
re2c:flags:tags = 0;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int valid_token(const char *&YYCURSOR)
{
  const char *YYMARKER;
  const char *tok = YYCURSOR;

  /*!re2c
    "byr:"[^\x20]+" " {
return 1;
    }
    "iyr:"[^\x20]+" " {
return 1;
    }
    "eyr:"[^\x20]+" " {
return 1;
    }
    "ecl:"[^\x20]+" " {
return 1;
    }
    "hgt:"[^\x20]+" " {
return 1;
    }
    "hcl:"[^\x20]+" " {
return 1;
    }
    "pid:"[^\x20]+" " {
return 1;
    }
    "cid:"[^\x20]+" " {
return 0;
    }
    * {
return -1;
    }
   */
}

bool valid_passport(const std::string &data)
{
  int valid_count = 0;
  const char *pos = data.data();
  while (true)
  {
    int val = valid_token(pos);
    switch (val)
    {
    case -1:
      return false;
    default:
      valid_count += val;
    }
    if (pos >= data.data() + data.size())
    {
      break;
    }
  }
  return valid_count >= 7;
}

int main(int argc, char **argv)
{
  int count = 0;
  std::string passport_data, line;
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  while (!in.eof())
  {
    std::getline(in, line);
    if (line.empty())
    {
      // end of passport
      bool res = valid_passport(passport_data);
      if (res)
      {
        ++count;
      }
      passport_data.clear();
    }
    else
    {
      passport_data += line + " ";
    }
  }
  std::cout << count << std::endl;
}
