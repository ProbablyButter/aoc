/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = "char";
re2c:yyfill:enable = 0;
re2c:flags:tags = 0;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"

#include "whereami.h"

#include <iostream>

std::filesystem::path get_resource_path(const std::string &name)
{
  std::string p(wai_getExecutablePath(nullptr, 0, nullptr), '\0');
  int dir_name_length;
  wai_getExecutablePath(&p[0], p.size(), &dir_name_length);
  p.resize(dir_name_length);
  std::filesystem::path res(p);
  res /= name;
  return res;
}

namespace aoc
{
  long long svtoll(const std::string_view &str, size_t base)
  {
    auto YYCURSOR = str.begin();
    auto YYLIMIT = str.end();

    long long sign = 0;
    long long res = 0;

  start:
    /*!re2c
"-" {
if(sign) return 0;
sign = -1;
goto start;
}
"+" {
if(sign) return 0;
sign = 1;
goto start;
}
[\x20\t\f\n\r\v]+ {
goto start;
}
'0x' {
if(base) return 0;
base = 16;
goto process_hex;
}
'0b' {
if(base) return 0;
base = 2;
goto process_bin;
}
'0' {
if(base) return 0;
base = 8;
goto process_oct;
}
[0-9a-fA-F] {
switch(base)
{
case 0:
case 10:
base = 10;
--YYCURSOR;
goto process_dec;

case 2:
--YYCURSOR;
goto process_bin;

case 8:
--YYCURSOR;
goto process_oct;
case 16:
--YYCURSOR;
goto process_hex;
default:
return 0;
}
}
   * {
goto done;
}
     */

  process_bin:
  /*!re2c
[0-1] {
res *= 2;
res += YYCURSOR[-1] - '0';
goto process_bin;
}
   * {
goto done;
}
   */
  process_oct:
  /*!re2c
[0-7] {
res *= 8;
res += YYCURSOR[-1] - '0';
goto process_oct;
}
   * {
goto done;
}
   */
  process_dec:
  /*!re2c
[0-9] {
res *= 10;
res += YYCURSOR[-1] - '0';
goto process_dec;
}
   * {
goto done;
}
   */
  process_hex:
    /*!re2c
   [0-9] {
   res *= 16;
   res += YYCURSOR[-1] - '0';
goto process_hex;
   }
[a-f] {
   res *= 16;
   res += YYCURSOR[-1] - 'a' + 0xall;
goto process_hex;
   }
[A-F] {
   res *= 16;
   res += YYCURSOR[-1] - 'A' + 0xall;
goto process_hex;
   }
      * {
   goto done;
   }
      */
  done:
    if (sign == 0)
    {
      sign = 1;
    }
    return sign * res;
  }
} // namespace aoc
