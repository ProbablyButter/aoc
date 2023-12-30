/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "json.hpp"

#include "string_tools.hpp"

#include <iostream>
#include <iterator>

namespace aoc {

enum class token_type {
  none,
  cbrace_l,
  cbrace_r,
  sbrace_l,
  sbrace_r,
  comma,
  string,
  integer,
  real,
  colon,
  null
};

static bool is_int(const char *YYCURSOR, const char *stop) {
  const char *YYMARKER;
  const char *d1;
  /*!stags:re2c format = 'const char *@@;'; */
parse_start:
  /*!re2c
[+-]? [0-9]+ @d1 {
  return (d1 == stop);
}
* {
  return false;
}
  */
}

static const char *parse_token(const char *YYCURSOR, std::any &value,
                               token_type &type) {
  const char *d0, *d1;
  const char *YYMARKER;
  std::string tmp;
  /*!stags:re2c format = 'const char *@@;'; */
parse_start:
  /*!re2c
"\"" {
  type = token_type::string;
  goto parse_string;
}
@d0 [+-]? (([0-9]* "."? [0-9]+) | ([0-9]+ "."? [0-9]*)) ([eE] "-"? [0-9]+)? @d1
{
  if(is_int(d0, d1)) {
    value = aoc::to_int<int64_t>(d0, d1);
    type = token_type::integer;
  }
  else
  {
    value = aoc::to_double(d0, d1);
    type = token_type::real;
  }
  return YYCURSOR;
}
[\x20\f\t\n\r\v]+ {
  goto parse_start;
}
[\x00] {
  type = token_type::none;
  return YYCURSOR;
}
"{" {
  type = token_type::cbrace_l;
  return YYCURSOR;
}
"}" {
  type = token_type::cbrace_r;
  return YYCURSOR;
}
"[" {
  type = token_type::sbrace_l;
  return YYCURSOR;
}
"]" {
  type = token_type::sbrace_r;
  return YYCURSOR;
}
"null" {
  type = token_type::null;
  value.reset();
  return YYCURSOR;
}
"," {
  type = token_type::comma;
  return YYCURSOR;
}
":" {
  type = token_type::colon;
  return YYCURSOR;
}
* {
  throw std::runtime_error("invalid token");
}
   */

parse_string:
  /*!re2c
"\\t" {
  tmp.push_back('\t');
  goto parse_string;
}
"\\n" {
  tmp.push_back('\n');
  goto parse_string;
}
"\\r" {
  tmp.push_back('\r');
  goto parse_string;
}
"\\b" {
  tmp.push_back('\b');
  goto parse_string;
}
"\\f" {
  tmp.push_back('\f');
  goto parse_string;
}
"\\x" @d0 [0-9a-fA-F]{1,2} @d1 {
  tmp.push_back(static_cast<char>(aoc::to_int<unsigned char>(d0, d1, 16)));
  goto parse_string;
}
"\\U" @d0 [0-9a-fA-F]{4} @d1 {
  // TODO: convert code to UTF8?
  uint16_t code = aoc::to_int<uint16_t>(d0, d1, 16);
  goto parse_string;
}
"\\\"" {
  tmp.push_back('\n');
  goto parse_string;
}
"\\'" {
  tmp.push_back('\'');
  goto parse_string;
}
"\\\\" {
  tmp.push_back('\\');
  goto parse_string;
}
"\\" {
  // can't have a stand-alone backslash
  throw std::runtime_error("invalid escape token");
}
"\"" {
  value = tmp;
  type = token_type::string;
  return YYCURSOR;
}
[\x00] {
  throw std::runtime_error("missing closing double quote");
}
@d0 [\x01-\xff] {
  tmp.push_back(*d0);
  goto parse_string;
}
   */
  throw std::runtime_error("shouldn't reach here");
}

static const char *parse_object(const char *YYCURSOR, json::object_type &res);
static const char *parse_array(const char *YYCURSOR, json::array_type &res);

static const char *parse_object(const char *YYCURSOR, json::object_type &res) {
  // assume we're just after the opening brace
  std::string curr_name;
  token_type type;
  std::any value;

parse_name:
  YYCURSOR = parse_token(YYCURSOR, value, type);
  switch (type) {
  case token_type::cbrace_r:
    return YYCURSOR;
  case token_type::string:
    curr_name = std::any_cast<std::string>(value);
    // look for colon
    YYCURSOR = parse_token(YYCURSOR, value, type);
    if (type != token_type::colon) {
      throw std::runtime_error(
          "unexpected token looking for colon before value");
    }
    break;
  default:
    throw std::runtime_error("unexpected token looking for attribute name");
  }

parse_value:
  YYCURSOR = parse_token(YYCURSOR, value, type);
  switch (type) {
  case token_type::string:
  case token_type::integer:
  case token_type::real:
  case token_type::null:
    res.emplace(std::move(curr_name), std::move(value));
    break;
  case token_type::cbrace_l: {
    // start of an object
    auto tmp = res.emplace(std::move(curr_name), json::object_type{});
    json::object_type &sub = *std::any_cast<json::object_type>(&tmp->second);
    YYCURSOR = parse_object(YYCURSOR, sub);
  } break;
  case token_type::sbrace_l: {
    // start of an array
    auto tmp = res.emplace(std::move(curr_name), json::array_type{});
    json::array_type &sub = *std::any_cast<json::array_type>(&tmp->second);
    YYCURSOR = parse_array(YYCURSOR, sub);
  } break;
  default:
    throw std::runtime_error("unexpected token looking for object value");
  }

parse_ending:
  // are we ending the object or looking for more values?
  YYCURSOR = parse_token(YYCURSOR, value, type);
  switch (type) {
  case token_type::cbrace_r:
    // done
    return YYCURSOR;
  case token_type::comma:
    goto parse_name;
  default:
    throw std::runtime_error("unexpected token looking for object close");
  }
}

static const char *parse_array(const char *YYCURSOR, json::array_type &res) {
  // asume we're just after the opening brace
  token_type type;
  std::any value;

parse_value:
  YYCURSOR = parse_token(YYCURSOR, value, type);
  switch (type) {
  case token_type::string:
  case token_type::integer:
  case token_type::real:
  case token_type::null:
    res.emplace_back(std::move(value));
    break;
  case token_type::cbrace_l: {
    // start of an object
    res.emplace_back(json::object_type{});
    json::object_type &sub = *std::any_cast<json::object_type>(&res.back());
    YYCURSOR = parse_object(YYCURSOR, sub);
  } break;
  case token_type::sbrace_l: {
    // start of an array
    res.emplace_back(json::array_type{});
    json::array_type &sub = *std::any_cast<json::array_type>(&res.back());
    YYCURSOR = parse_array(YYCURSOR, sub);
  } break;
  default:
    throw std::runtime_error("unexpected token looking for array value");
  }

parse_ending:
  // are we ending the array or looking for more values?
  YYCURSOR = parse_token(YYCURSOR, value, type);
  switch (type) {
  case token_type::sbrace_r:
    // done
    return YYCURSOR;
  case token_type::comma:
    goto parse_value;
  default:
    throw std::runtime_error("unexpected token looking for array close");
  }
}

void json::parse(std::istream &in) {
  // TODO: switch this to a token-based parser design would be much better
  std::string data(std::istreambuf_iterator<char>{in},
                   std::istreambuf_iterator<char>{});

  const char *YYCURSOR = data.data();
  std::any value;
  token_type type;

  YYCURSOR = parse_token(YYCURSOR, value, type);
  switch (type) {
  case token_type::cbrace_l: {
    // start of an object
    root = json::object_type{};
    json::object_type &sub = *std::any_cast<json::object_type>(&root);
    YYCURSOR = parse_object(YYCURSOR, sub);
  } break;
  case token_type::sbrace_l: {
    // start of an array
    root = json::array_type{};
    json::array_type &sub = *std::any_cast<json::array_type>(&root);
    YYCURSOR = parse_array(YYCURSOR, sub);
  } break;
  default:
    throw std::runtime_error("missing root node open brace");
  }
  // make sure we have no more tokens
  YYCURSOR = parse_token(YYCURSOR, value, type);
  if (type != token_type::none) {
    throw std::runtime_error("extra tokens after closing root }");
  }
}

void json::dump(std::ostream &out) const {
  // TODO
}
} // namespace aoc
