/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = "char";
re2c:yyfill:enable = 0;
re2c:flags:tags = 0;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"

#import <fstream>
#import <iostream>
#include <sstream>
#import <string>
#include <string_view>
#include <unordered_set>
#import <vector>

enum token_type { BYR, EYR, ECL, HGT, HCL, PID, CID, NONE };

token_type next_token(const char *YYCURSOR, const char *&val_start, const char *&val_stop)
{
  token_type res = NONE;

  const char *YYMARKER;
  const char *tok = YYCURSOR;

  /*!re2c
    "byr:" {
res = BYR;
    }
    "eyr:" {
res = EYR;
    }
    "ecl:" {
res = ECL;
    }
    "hgt:" {
res = HGT;
    }
    "hcl:" {
res = HCL;
    }
    "pid:" {
res = PID;
    }
    "cid:" {
res = CID;
    }
    * {
res = NONE;
    }
   */
  val_start = val_stop = YYCURSOR;

  if (res != NONE)
  {
    while (*YYCURSOR && *YYCURSOR != ' ')
    {
      ++YYCURSOR;
    }
    val_stop = YYCURSOR;
  }

  return res;
}

bool valid_passport(const std::string &data)
{
  std::istringstream in(data);
  std::string seq;
  int valid_count = 0;
  while (true)
  {
    in >> seq;
    if (in.eof())
    {
      break;
    }
    if (seq.empty())
    {
      continue;
    }
    if (seq.size() < 4 || seq[3] != ':')
    {
      // can't possibly be valid
      return false;
    }
    switch (seq[0])
    {
    case 'b':
      if (seq[1] == 'y' && seq[2] == 'r')
      {
        // "byr"
        ++valid_count;
        break;
      }
      return false;
    case 'i':
      if (seq[1] == 'y' && seq[2] == 'r')
      {
        // "iyr"
        ++valid_count;
        break;
      }
    case 'e':
      if (seq[1] == 'y' && seq[2] == 'r')
      {
        // "eyr"
        ++valid_count;
        break;
      }
      else if (seq[1] == 'c' && seq[2] == 'l')
      {
        // "ecl"
        ++valid_count;
        break;
      }
      return false;
    case 'h':
      if (seq[1] == 'g' && seq[2] == 't')
      {
        // "hgt"
        ++valid_count;
        break;
      }
      else if (seq[1] == 'c' && seq[2] == 'l')
      {
        // "hcl"
        ++valid_count;
        break;
      }
      break;
    case 'p':
      if (seq[1] == 'i' && seq[2] == 'd')
      {
        // "pid"
        ++valid_count;
        break;
      }
      return false;
    case 'c':
      if (seq[1] == 'i' && seq[2] == 'd')
      {
        // "cid", ignore this
        break;
      }
      return false;
    default:
      return false;
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
