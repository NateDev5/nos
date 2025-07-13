#pragma once

#include <utils/types.h>

// http://www.osdever.net/FreeVGA/home.htm
// http://www.osdever.net/FreeVGA/vga/crtcreg.htm#0A
// http://www.osdever.net/FreeVGA/vga/vgareg.htm

#define BASE_VID_MEM            0xB8000
#define SCRN_WIDTH              80
#define SCRN_HEIGHT             25
#define SCRN_SIZE               SCRN_HEIGHT * SCRN_WIDTH

#define ADDRESS_REGISTER_PORT   0x3D4
#define DATA_REGISTER_PORT      0x3D5

#define CURSOR_START_REGISTER   0x0A
#define CURSOR_END_REGISTER     0x0B

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

// shift 4 bits to the left so the first 4 bits are the first color and then mask the last 4 bits to the second color
#define FORMAT(back,fore) (back << 4) | (fore)

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

    void enableCursor (IN uint8 cursorStart, IN uint8 cursorEnd);
    void disableCursor ();
    void setCursorPos (IN uint8 x, IN uint8 y);
    
    void test();
    void putchar (IN int8 _char);
    void fputchar (IN int8 _char, IN uint8 format);
    void putstr (IN cstr string);
    void fputstr (IN cstr string, IN uint8 format);
    //void printchar (IN int8 _char, IN uint8 format);
    //void print(IN const cstr msg, IN uint8 format);
    //void println(IN const cstr msg, IN uint8 format);
    //void printuint32 (IN uint32 value, IN int32 base, IN uint8 format);
    //void printf(IN cstr msg, IN uint8 format, IN ...);
    void setBackgroundColor (IN uint8 color);
    void setForegroundColor (IN uint8 color);
    void clearScreen ();
    void errorScreen (IN const cstr msg);
}