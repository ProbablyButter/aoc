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
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

void parse_ingredients(const char *YYCURSOR,
                       std::unordered_set<std::string> &ingredients,
                       std::unordered_set<std::string> &allergens)
{
  const char *YYMARKER;
  const char *start;
  const char *stop;

  const char *init = YYCURSOR;

  /*!stags:re2c format = 'const char *@@;'; */

parse_start:
  /*!re2c
@start [a-z]+ @stop {
ingredients.emplace(start, stop - start);
goto parse_start;
}
'(contains' {
goto parse_allergens;
}
[\x00] {
return;
}
* {
goto parse_start;
}
   */

parse_allergens:
  /*!re2c
  @start [a-z]+ @stop {
  allergens.emplace(start, stop - start);
  goto parse_allergens;
  }
  [\x00] {
  return;
  }
  * {
  goto parse_allergens;
  }
   */
}

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;

  std::vector<std::unordered_set<std::string>> all_ingredients;
  std::vector<std::unordered_set<std::string>> all_allergens;

  // parse input
  {
    while (true)
    {
      std::getline(in, line);
      if (in.eof())
      {
        break;
      }
      all_ingredients.emplace_back();
      all_allergens.emplace_back();
      parse_ingredients(line.data(), all_ingredients.back(), all_allergens.back());
    }
  }

  // figure out which ingredients can't possibly be allergens
  // build map from allergen to foods known to contain allergen
  std::unordered_map<std::string, std::vector<int>> contains_allergen;
  for (size_t i = 0; i < all_allergens.size(); ++i)
  {
    for (auto &v : all_allergens[i])
    {
      auto iter = contains_allergen.find(v);
      if (iter == contains_allergen.end())
      {
        iter = contains_allergen.emplace(v, std::vector<int>{}).first;
      }
      iter->second.push_back(i);
    }
  }

  // figure out what ingredients could possibly be an allergen
  std::unordered_map<std::string, std::unordered_set<std::string>> allergen_to_ingredient;
  for (auto &v : contains_allergen)
  {
    auto possible_ingredients = all_ingredients[v.second.at(0)];
    for (size_t i = 1; i < v.second.size(); ++i)
    {
      for (auto iter = possible_ingredients.begin(); iter != possible_ingredients.end();)
      {
        auto iter2 = all_ingredients[v.second[i]].find(*iter);
        if (iter2 == all_ingredients[v.second[i]].end())
        {
          // this ingredient can't be the allergen v
          iter = possible_ingredients.erase(iter);
        }
        else
        {
          // still a candidate
          ++iter;
        }
      }
    }
    allergen_to_ingredient[v.first] = possible_ingredients;
  }
  // now narrow down the search
  for (auto iter = allergen_to_ingredient.begin(); iter != allergen_to_ingredient.end();)
  {
    if (iter->second.size() == 1)
    {
      // found a mapping, remove from all other potential allergens
      bool any_removed = false;
      for (auto iter2 = allergen_to_ingredient.begin();
           iter2 != allergen_to_ingredient.end();
           ++iter2)
      {
        if (iter2 != iter)
        {
          if (iter2->second.erase(*iter->second.begin()))
          {
            any_removed = true;
          }
        }
      }
      if (any_removed)
      {
        // start over
        iter = allergen_to_ingredient.begin();
      }
      else
      {
        ++iter;
      }
    }
    else
    {
      ++iter;
    }
  }

  // now to actually do the challenge: remove the allergen from all_ingredients
  long long count = 0;
  for(auto& v : all_ingredients)
  {
    for(auto&w : allergen_to_ingredient)
    {
      v.erase(*w.second.begin());
    }
    count += v.size();
  }
  std::cout << count << std::endl;
}
