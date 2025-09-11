#pragma once

#include <utils/types.h>

namespace Kernel::Terminal {
struct TerminalInfo {
    uint32_t width;
    uint32_t height;

    uint32_t x;
    uint32_t y;
};

void init();
void print_char (IN char _char);
void print(IN CONST_CHAR_PTR str);
void clear ();
}
