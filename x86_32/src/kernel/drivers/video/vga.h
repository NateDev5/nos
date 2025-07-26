#pragma once

#include <utils/types.h>
// http://www.osdever.net/FreeVGA/home.htm
// http://www.osdever.net/FreeVGA/vga/crtcreg.htm#0A
// http://www.osdever.net/FreeVGA/vga/vgareg.htm

#define START_VID_MEM                 0xA0000
#define BASE_VID_MEM                  0xB8000
#define END_VID_MEM                   0xC0000

#define TEXT_MODE_SCREEN_WIDTH        80
#define TEXT_MODE_SCREEN_HEIGHT       25
#define TEXT_MODE_SCREEN_SIZE         (TEXT_MODE_SCREEN_WIDTH * TEXT_MODE_SCREEN_HEIGHT)

#define ADDRESS_REGISTER_PORT         0x3D4
#define DATA_REGISTER_PORT            0x3D5

#define MAX_SCAN_LINE_REGISTER        0x09
#define CURSOR_START_REGISTER         0x0A
#define CURSOR_END_REGISTER           0x0B
#define CURSOR_LOCATION_HIGH_REGISTER 0x0E
#define CURSOR_LOCATION_LOW_REGISTER  0x0F

#define CURSOR_DISABLED               0x20

#define FIRST_5_BIT_MASK              0x1F

// shift 4 bits to the left so the first 4 bits are the first color and then
// mask the last 4 bits to the second color
#define FORMAT(back, fore)            (back << 4) | (fore)

#define BASE_FMT                      FORMAT(0x0, 0xF)

namespace Drivers::VGA {
enum Color {
    BLACK = 0x0,
    BLUE = 0x1,
    GREEN = 0x2,
    CYAN = 0x3,
    RED = 0x4,
    MAGENTA = 0x5,
    BROWN = 0x6,
    WHITE = 0x7,
    GRAY = 0x8,
    LBLUE = 0x9,
    LGREEN = 0xA,
    LCYAN = 0xB,
    LRED = 0xC,
    LMAGENTA = 0xD,
    YELLOW = 0xE,
    BWHITE = 0xF
};

enum CursorStyle { UNDER, LARGE };

void modify_register(IN uint8_t reg, IN uint8_t data);

void enable_cursor(IN CursorStyle cursor_style);
void disable_cursor();

void set_cursor_offset(IN uint16_t offset);
void set_cursor_pos(IN uint16_t x, IN uint16_t y);

void put_char(IN char _char);
void f_put_char(IN char _char, IN uint8_t color);

void put_str(IN CONST_CHAR_PTR string);
void f_put_str(IN CONST_CHAR_PTR string, IN uint8_t color);

void f_insert_char_at(IN char _char, IN uint16_t offset);

void remove_char_at(IN uint16_t offset);

void set_background_color(IN uint8_t color);
void set_foreground_color(IN uint8_t color);

void scroll_up();

void clear_screen();
void test();

uint16_t get_current_offset();
uint16_t get_screen_width();
uint16_t get_screen_heigth();
uint16_t get_screen_size();
} // namespace Drivers::VGA
