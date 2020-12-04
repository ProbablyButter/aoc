#ifndef _RE2C_AST_INPUT_
#define _RE2C_AST_INPUT_

#include <stdio.h>
#include <string>
#include <vector>
#include <cstdint>
#include "src/util/forbid_copy.h"

namespace re2c {

struct Input
{
    FILE *file;
    std::string name;
    std::string escaped_name;
    const char *so; // start offset in buffer
    const char *eo; // end offset in buffer
    uint32_t line;

    Input();
    ~Input();
    bool open(const std::string &filename, const std::string *parent
        , const std::vector<std::string> &incpaths);

    FORBID_COPY(Input);
};

} // namespace re2c

#endif // _RE2C_AST_INPUT_
