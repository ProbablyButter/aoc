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
#include <list>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// true player 1 wins, false player 2 wins
bool play_game(std::list<int> &player1, std::list<int> &player2)
{
  std::vector<std::pair<std::list<int>, std::list<int>>> history;
  while (true)
  {
    // try to draw a card from the top
    if (player1.empty() || player2.empty())
    {
      return player2.empty();
    }
    // check to see if there has been a previous round where the exact same hand has been
    // played
    for (auto &h : history)
    {
      if (h.first == player1 && h.second == player2)
      {
        // player 1 automatically wins
        return true;
      }
    }
    // add entry to history
    history.emplace_back(player1, player2);
    // play the game
    if (player1.size() >= player1.front() + 1 && player2.size() >= player2.front() + 1)
    {
      // begin a recursive round
      auto iter1 = player1.begin();
      std::advance(iter1, 1);
      auto iter2 = iter1;
      std::advance(iter2, player1.front());
      std::list<int> copy1(iter1, iter2);

      iter1 = player2.begin();
      std::advance(iter1, 1);
      iter2 = iter1;
      std::advance(iter2, player2.front());
      std::list<int> copy2(iter1, iter2);
      bool sub_res = play_game(copy1, copy2);
      if (sub_res)
      {
        // player 1 wins
        player1.push_back(player1.front());
        player1.push_back(player2.front());
      }
      else
      {
        // player 2 wins
        player2.push_back(player2.front());
        player2.push_back(player1.front());
      }
    }
    else
    {
      // normal game rules
      if (player1.front() > player2.front())
      {
        // player 1 wins
        player1.push_back(player1.front());
        player1.push_back(player2.front());
      }
      else if (player1.front() == player2.front())
      {
        // tie
        player1.push_back(player1.front());
        player2.push_back(player2.front());
      }
      else
      {
        // player 2 wins
        player2.push_back(player2.front());
        player2.push_back(player1.front());
      }
    }
    player1.pop_front();
    player2.pop_front();
  }
}

long long get_score(const std::list<int> &deck)
{
  long long res = 0;
  long long i = 1;
  for (auto iter = deck.rbegin(); iter != deck.rend(); ++iter)
  {
    res += *iter * i;
    ++i;
  }
  return res;
}

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;

  std::list<int> player1;
  std::list<int> player2;

  // parse input
  // player 1
  std::getline(in, line);
  {
    while (true)
    {
      std::getline(in, line);
      if (in.eof())
      {
        break;
      }
      if (line.empty())
      {
        break;
      }
      player1.push_back(atoi(line.data()));
    }
  }
  // player 2
  std::getline(in, line);
  {
    while (true)
    {
      std::getline(in, line);
      if (in.eof())
      {
        break;
      }
      if (line.empty())
      {
        break;
      }
      player2.push_back(atoi(line.data()));
    }
  }

  play_game(player1, player2);
  long long res = 0;
  if (player1.empty())
  {
    res = get_score(player2);
  }
  else
  {
    res = get_score(player1);
  }
  std::cout << res << std::endl;
}
