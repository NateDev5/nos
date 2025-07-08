#pragma once

#include "../utils/types.h"
#include "../interrupts/idt.h"

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

void initKeyboard (IN bool verbose);
int8 readKey();
void readLine (OUT cstr outStr);

__attribute__((interrupt))
void IRQ1_keyboardHandler (IN InterruptFrame* frame);

typedef struct
{

} KEYPRESS_INFO;