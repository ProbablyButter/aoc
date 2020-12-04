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
  static std::unordered_set<std::string> valid_ecl = {
    "amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
  int valid_count = 0;
  const char *pos = data.data();
  const char *val_start = pos;
  while (true)
  {
    token_type type = next_token(pos, val_start, pos);
    std::string_view value(val_start, pos - val_start);
    switch (type)
    {
    case BYR:
    {
      if (value.size() != 4)
      {
        std::cout << "invalid byr: " << value << std::endl;
        return false;
      }
      auto year = aoc::svtoll(value);
      if (year < 1920 || year > 2002)
      {
        std::cout << "invalid byr: " << value << std::endl;
        return false;
      }
    }
    break;
    case IYR:
    {
      if (value.size() != 4)
      {
        std::cout << "invalid iyr: " << value << std::endl;
        return false;
      }
      auto year = aoc::svtoll(value);
      if (year < 2010 || year > 2020)
      {
        std::cout << "invalid iyr: " << value << std::endl;
        return false;
      }
    }
    break;
    case EYR:
    {
      if (value.size() != 4)
      {
        std::cout << "invalid eyr: " << value << std::endl;
        return false;
      }
      auto year = aoc::svtoll(value);
      if (year < 2020 || year > 2030)
      {
        std::cout << "invalid eyr: " << value << std::endl;
        return false;
      }
    }
    break;
    case ECL:
    {
      if (valid_ecl.find(std::string(value)) == valid_ecl.end())
      {
        std::cout << "invalid ecl: " << value << std::endl;
        return false;
      }
    }
    break;
    case HGT:
    {
      if (value.size() < 3)
      {
        std::cout << "invalid hgt: " << value << std::endl;
        return false;
      }
      auto hgt = value.substr(0, value.size() - 2);
      for (auto c : hgt)
      {
        if (!std::isdigit(c))
        {
          std::cout << "invalid hgt: " << value << std::endl;
          return false;
        }
      }
      long long val = aoc::svtoll(hgt);
      if (value[value.size() - 2] == 'c' && value[value.size() - 1] == 'm')
      {
        if (val < 150 || val > 193)
        {
          std::cout << "invalid hgt: " << value << std::endl;
          return false;
        }
      }
      else if (value[value.size() - 2] == 'i' && value[value.size() - 1] == 'n')
      {
        if (val < 59 || val > 76)
        {
          std::cout << "invalid hgt: " << value << std::endl;
          return false;
        }
      }
      else
      {
        std::cout << "invalid hgt: " << value << std::endl;
        return false;
      }
    }
    break;
    case HCL:
    {
      if (value.size() != 7 || value[0] != '#')
      {
        std::cout << "invalid hcl: " << value << std::endl;
        return false;
      }
      for (int i = 1; i < 7; ++i)
      {
        if (!std::isxdigit(value[i]))
        {
          std::cout << "invalid hcl: " << value << std::endl;
          return false;
        }
      }
    }
    break;
    case PID:
    {
      if (value.size() != 9)
      {
        std::cout << "invalid pid: " << value << std::endl;
        return false;
      }
      for (int i = 9; i < 9; ++i)
      {
        if (!std::isdigit(value[i]))
        {
          std::cout << "invalid pid: " << value << std::endl;
          return false;
        }
      }
    }
    break;
    case CID:
      break;
    case NONE:
      return false;
    }
    ++pos;
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
