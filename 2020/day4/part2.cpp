#include "aoc.hpp"

#include <cstdlib>
#import <fstream>
#import <iostream>
#include <sstream>
#import <string>
#include <string_view>
#include <unordered_set>
#import <vector>

struct passport
{
  int byr = 0, iyr = 0, eyr = 0, hgt = 0;
  bool cm = false;
  std::string ecl, hcl, pid;
};

bool valid_passport(const std::string &data)
{
  static std::unordered_set<std::string> valid_eye_colors = {
    "amb", "blu", "brn", "grn", "gry", "hzl", "oth"};

  int valid_count = 0;

  passport p;

  std::istringstream in(data);
  std::string seq;
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
        if (seq.size() != 3 + 1 + 4)
        {
          return false;
        }
        // validate year
        int year = atoi(&seq[4]);
        if (year >= 1920 && year <= 2002)
        {
          ++valid_count;
          p.byr = year;
        }
        else
        {
          return false;
        }
        break;
      }
      return false;
    case 'i':
      if (seq[1] == 'y' && seq[2] == 'r')
      {
        // "iyr"
        if (seq.size() != 3 + 1 + 4)
        {
          return false;
        }
        // validate year
        int year = atoi(&seq[4]);
        if (year >= 2010 && year <= 2020)
        {
          ++valid_count;
          p.iyr = year;
        }
        else
        {
          return false;
        }
        break;
      }
    case 'e':
      if (seq[1] == 'y' && seq[2] == 'r')
      {
        // "eyr"
        if (seq.size() != 3 + 1 + 4)
        {
          return false;
        }
        // validate year
        int year = atoi(&seq[4]);
        if (year >= 2020 && year <= 2030)
        {
          ++valid_count;
          p.eyr = year;
        }
        else
        {
          return false;
        }
        break;
      }
      else if (seq[1] == 'c' && seq[2] == 'l')
      {
        // "ecl"
        if (seq.size() != 3 + 1 + 3)
        {
          return false;
        }
        p.ecl = seq.substr(4);
        if (valid_eye_colors.find(p.ecl) == valid_eye_colors.end())
        {
          return false;
        }
        ++valid_count;
        break;
      }
      return false;
    case 'h':
      if (seq[1] == 'g' && seq[2] == 't')
      {
        // "hgt"
        if (seq[seq.size() - 2] == 'c' && seq[seq.size() - 1] == 'm')
        {
          // cm
          int hgt = atoi(seq.substr(4, seq.size() - 2 - 4).c_str());
          if (hgt >= 150 && hgt <= 193)
          {
            p.hgt = hgt;
            p.cm = true;
            ++valid_count;
          }
          else
          {
            return false;
          }
        }
        else if (seq[seq.size() - 2] == 'i' && seq[seq.size() - 1] == 'n')
        {
          int hgt = atoi(seq.substr(4, seq.size() - 2).c_str());
          if (hgt >= 59 && hgt <= 76)
          {
            p.hgt = hgt;
            ++valid_count;
          }
          else
          {
            return false;
          }
        }
        else
        {
          return false;
        }
        break;
      }
      else if (seq[1] == 'c' && seq[2] == 'l')
      {
        // "hcl"
        if (seq.size() == 3 + 1 + 1 + 6)
        {
          if (seq[4] == '#')
          {
            for (size_t i = 0; i < 6; ++i)
            {
              if (!std::isxdigit(seq[4 + 1 + i]))
              {
                return false;
              }
            }
            p.hcl = seq.substr(4 + 1);
            ++valid_count;
          }
          else
          {
            return false;
          }
        }
        else
        {
          return false;
        }
        break;
      }
      break;
    case 'p':
      if (seq[1] == 'i' && seq[2] == 'd')
      {
        // "pid"
        if (seq.size() == 3 + 1 + 9)
        {
          for (size_t i = 0; i < 9; ++i)
          {
            if (!std::isdigit(seq[4 + i]))
            {
              return false;
            }
          }
          p.pid = seq.substr(4);
          ++valid_count;
        }
        else
        {
          return false;
        }
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
  if (valid_count >= 7)
  {
    std::cout << "byr: " << p.byr << " iyr: " << p.iyr << " eyr: " << p.eyr << " hgt: " << p.hgt;
    if (p.cm)
    {
      std::cout << "cm";
    }
    else
    {
      std::cout << "in";
    }
    std::cout << " ecl: " << p.ecl << " hcl: " << p.hcl << " pid: " << p.pid << std::endl;
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
        // std::cout << passport_data << std::endl;
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
