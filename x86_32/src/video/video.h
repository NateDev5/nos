#pragma once
#include "../utils/types.h"

#define BASE_VID_MEM   0xB8000
#define SCRN_WIDTH     80
#define SCRN_HEIGHT    25
#define SCRN_SIZE      SCRN_HEIGHT * SCRN_WIDTH

// shift 4 bits to the left so the first 4 bits are the first color and then mask the last 4 bits to the second color

#define PRNT_FORMAT(back,fore) (back << 4) | (fore)

#define BLACK       0x0
#define BLUE        0x1
#define GREEN       0x2
#define CYAN        0x3
#define RED         0x4
#define MAGENTA     0x5
#define BROWN       0x6
#define WHITE       0x7
#define GRAY        0x8
#define LBLUE       0x9
#define LGREEN      0xA
#define LCYAN       0xB
#define LRED        0xC
#define LMAGENTA    0xD
#define YELLOW      0xE
#define BWHITE      0xF

void test();
void print(const cstr msg, uint8 format);
void println(const cstr msg, uint8 format);
void setBackgroundColor (uint8 color);
void setForegroundColor (uint8 color);
void clearScreen ();