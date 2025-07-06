#pragma once

#include "../utils/types.h"

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_REGISTER 0x64

static const char scanCodes[128] = {
    0, 0,
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
    '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
    '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, 0, 0, ' '};

bool initPS2Controller();
int8 readKey();

typedef struct
{

} KEYPRESS_INFO;