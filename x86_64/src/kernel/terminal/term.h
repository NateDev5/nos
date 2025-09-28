#pragma once

#include <utils/types.h>

// hard coded
#define TEMP_TERM_WIDTH 160
#define TEMP_TERM_HEIGHT 100

#define DIRTY 1
#define NOT_DIRTY 0

namespace Kernel::Terminal {
struct TerminalInfo {
    uint32_t width;
    uint32_t height;

    uint32_t x;
    uint32_t y;
};

struct TerminalCell {
    char unicode;
    uint8_t dirty;
};

void init();
void print_char (IN char _char);
void print(IN CONST_CHAR_PTR str);
void modify_cell (IN uint32_t x, IN uint32_t y, IN char unicode);
void render_term();
void scroll_up();
void clear ();
}
