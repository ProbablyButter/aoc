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
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

enum token_type {
  BYR = 0,
  IYR = 1,
  EYR = 2,
  ECL = 3,
  HGT = 4,
  HCL = 5,
  PID = 6,
  CID = 7,
  NONE = -1
};

token_type next_token(const char *YYCURSOR, const char *&val_start, const char *&val_stop)
{
  token_type res = NONE;

  const char *YYMARKER;
  const char *tok = YYCURSOR;

  /*!re2c
    "byr:" {
res = BYR;
goto done;
    }
    "iyr:" {
res = IYR;
goto done;
    }
    "eyr:" {
res = EYR;
goto done;
    }
    "ecl:" {
res = ECL;
goto done;
    }
    "hgt:" {
res = HGT;
goto done;
    }
    "hcl:" {
res = HCL;
goto done;
    }
    "pid:" {
res = PID;
goto done;
    }
    "cid:" {
res = CID;
goto done;
    }
    * {
res = NONE;
goto done;
    }
   */
done:
#if 1
  val_start = val_stop = YYCURSOR;

  if (res != NONE)
  {
    while (*YYCURSOR && *YYCURSOR != ' ')
    {
      ++YYCURSOR;
    }
    val_stop = YYCURSOR;
  }
#endif

  return res;
}

bool valid_passport(const std::string &data)
{
  int valid_count = 0;
  const char *pos = data.data();
  const char *val_start = pos;
  while (true)
  {
    token_type type = next_token(pos, val_start, pos);
    std::string_view value(val_start, pos - val_start);
    ++pos;
    if (type == NONE)
    {
      return false;
    }
    valid_count += (type != CID);
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
