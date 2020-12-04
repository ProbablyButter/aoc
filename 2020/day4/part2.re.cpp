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
#include <string_view>
#include <vector>

#define  NONE 0
#define BYR 1
#define IYR 2
#define EYR 4
#define ECL 8
#define HGT 16
#define HCL 32
#define PID 64
#define CID 128

int valid_token(const char *&YYCURSOR)
{
  const char *YYMARKER;
  const char *tok = YYCURSOR;

  int min_year = 0, max_year = 0;
  int res = NONE;

  /*!re2c
    "byr:" {
min_year = 1920;
max_year = 2002;
res = BYR;
goto parse_year;
    }
    "iyr:" {
min_year = 2010;
max_year = 2020;
res = IYR;
goto parse_year;
    }
    "eyr:" {
min_year = 2020;
max_year = 2030;
res = EYR;
goto parse_year;
    }
    "ecl:"("amb"|"blu"|"brn"|"gry"|"grn"|"hzl"|"oth")" " {
return ECL;
    }
    "hgt:" {
goto parse_hgt;
    }
    "hcl:#"[0-9a-f]{6}" " {
return HCL;
    }
    "pid:"[0-9]{9}" " {
return PID;
    }
    "cid:"[^\x20]+" " {
return CID;
    }
    * {
return NONE;
    }
   */

parse_year:
  /*!re2c
[0-9]{4}" " {
long long year = aoc::svtoll(std::string_view(YYCURSOR - 5, 4));
if(year >= min_year && year <= max_year)
{
return res;
}
return NONE;
}
* {
return NONE;
}
   */

parse_hgt:
  long long hgt = -1;
  const char *save_pos = YYCURSOR;
  /*!re2c
 [0-9]+ {
 hgt = aoc::svtoll(std::string_view(save_pos, YYCURSOR - save_pos));
 goto parse_units;
 }
 * {
 return NONE;
 }
    */

parse_units:
  /*!re2c
"cm " {
if(hgt >= 150 && hgt <= 193)
{
return HGT;
}
return NONE;
}
"in " {
if(hgt >= 59 && hgt <= 76)
{
return HGT;
}
return NONE;
}
* {
return NONE;
}
   */
}

bool valid_passport(const std::string &data)
{
  int found = NONE;
  const char *pos = data.data();
  while (true)
  {
    int val = valid_token(pos);
    switch (val)
    {
    case NONE:
      return false;
    case CID:
      break;
    default:
      found |= val;
    }
    if (pos >= data.data() + data.size())
    {
      break;
    }
  }
  return found == (BYR | IYR | EYR | ECL | HGT | HCL | PID);
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
