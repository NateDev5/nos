#pragma once

#include <utils/types.h>
// http://www.osdever.net/FreeVGA/home.htm
// http://www.osdever.net/FreeVGA/vga/crtcreg.htm#0A
// http://www.osdever.net/FreeVGA/vga/vgareg.htm

#define START_VID_MEM                   0xA0000
#define BASE_VID_MEM                    0xB8000
#define END_VID_MEM                     0xC0000
#define SCRN_WIDTH                      80
#define SCRN_HEIGHT                     25
#define SCRN_SIZE                       SCRN_HEIGHT * SCRN_WIDTH
#define VID_MEM_BOUNDS                  (uint32)(BASE_VID_MEM + (SCRN_SIZE * 2))

#define ADDRESS_REGISTER_PORT           0x3D4
#define DATA_REGISTER_PORT              0x3D5

#define MAX_SCAN_LINE_REGISTER          0x09
#define CURSOR_START_REGISTER           0x0A
#define CURSOR_END_REGISTER             0x0B
#define CURSOR_LOCATION_HIGH_REGISTER   0x0E
#define CURSOR_LOCATION_LOW_REGISTER    0x0F

#define CURSOR_DISABLED                 0x20

#define FIRST_5_BIT_MASK                0x1F

// shift 4 bits to the left so the first 4 bits are the first color and then mask the last 4 bits to the second color
#define FORMAT(back,fore) (back << 4) | (fore)

#define BASE_FMT                        FORMAT(0x0,0xF)

/*
#define BLACK                   0x0
#define BLUE                    0x1
#define GREEN                   0x2
#define CYAN                    0x3
#define RED                     0x4
#define MAGENTA                 0x5
#define BROWN                   0x6
#define WHITE                   0x7
#define GRAY                    0x8
#define LBLUE                   0x9
#define LGREEN                  0xA
#define LCYAN                   0xB
#define LRED                    0xC
#define LMAGENTA                0xD
#define YELLOW                  0xE
#define BWHITE                  0xF
*/

namespace Drivers::VGA {
    enum Color {
        BLACK    =  0x0,
        BLUE     =  0x1,
        GREEN    =  0x2,
        CYAN     =  0x3,
        RED      =  0x4,
        MAGENTA  =  0x5,
        BROWN    =  0x6,
        WHITE    =  0x7,
        GRAY     =  0x8,
        LBLUE    =  0x9,
        LGREEN   =  0xA,
        LCYAN    =  0xB,
        LRED     =  0xC,
        LMAGENTA =  0xD,
        YELLOW   =  0xE,
        BWHITE   =  0xF
    };

    enum CursorStyle {
        UNDER,
        LARGE
    };

    void init ();

    void modify_register (IN uint8 reg, IN uint8 data);

    void enable_cursor (IN CursorStyle cursor_style);
    void disable_cursor ();
    void set_cursor_pos (IN uint16 offset);
    
    void putchar (IN int8 _char);
    void fputchar (IN int8 _char, IN uint8 format);

    void putstr (IN cstr string);
    void fputstr (IN cstr string, IN uint8 format);

    void fputchar_at (IN int8 _char, IN uint16 offset);

    void popchar ();

    void removechar_at (IN uint16 offset);

    void set_background_color (IN uint8 color);
    void set_foreground_color (IN uint8 color);

    void clear_screen ();
    void test();

    uint16 get_current_offset ();
}
