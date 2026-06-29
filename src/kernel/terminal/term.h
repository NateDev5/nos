#pragma once

#include <utils/types.h>

#include <shared/utils.h>

// hard coded
#define TEMP_TERM_WIDTH  160
#define TEMP_TERM_HEIGHT 100

#define DIRTY            1
#define NOT_DIRTY        0

#define ANSI_ESCAPE      27
#define ANSI_CSI         '['

#define ANSI_MAX_ARGS    5

#define BLUE             0x001D56BF
#define BLACK            0x0
#define RED              0x00BF1D1D
#define GREEN            0x003DBF1D
#define YELLOW           0x00BFA11D
#define MAGENTA          0x00BF1DB7
#define CYAN             0x001DBFB2
#define WHITE            0xFFFFFFFF
#define DEFAULT_FORE     WHITE
#define DEFAULT_BACK     0x20202020
namespace Kernel::Terminal {
struct TerminalInfo {
    uint32_t width;
    uint32_t height;

    uint32_t x;
    uint32_t y;

    bool ansi_escaped;     // ansi escape character detected
    bool ansi_sequence;    // ansi sequence is being parsed
    bool parsing_argument; // currently parsing argument

    char    ansi_arguments[ANSI_MAX_ARGS][4];
    uint8_t ansi_argument_ptr;
    uint8_t argument_digit_ptr;

    uint32_t backcolor;
    uint32_t forecolor;
};

struct TerminalCell {
    char     unicode;
    uint32_t forecolor;
    uint32_t backcolor;
    uint8_t  dirty;
};

enum AnsiCmd { Reset, Bold, Dim, Italic, Underline, Blinking, Inverse, Hidden, Strikethrough, Foreground, Background };

void init();
void print_char(IN char _char);
void print(IN CONST_CHAR_PTR str);
void modify_cell(IN uint32_t x, IN uint32_t y, IN char unicode);
void render_term(IN bool redraw = false);
void scroll_up();
void clear();

void apply_ansi_style();
void apply_ansi_rgb_color(IN AnsiCmd cmd);
void apply_ansi_preset_color(IN CHAR_PTR str);
} // namespace Kernel::Terminal
