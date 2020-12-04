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

  /*!re2c
    "byr:"[^\x20]+" " {
return BYR;
    }
    "iyr:"[^\x20]+" " {
return IYR;
    }
    "eyr:"[^\x20]+" " {
return EYR;
    }
    "ecl:"[^\x20]+" " {
return ECL;
    }
    "hgt:"[^\x20]+" " {
return HGT;
    }
    "hcl:"[^\x20]+" " {
return HCL;
    }
    "pid:"[^\x20]+" " {
return PID;
    }
    "cid:"[^\x20]+" " {
return CID;
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
