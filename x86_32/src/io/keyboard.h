#pragma once

#include "../utils/types.h"
#include "../interrupts/idt.h"

#define SCANCODE_ALT    0x11
#define SCANCODE_CTRL   0x14
#define SCANCODE_SHIFT  0x12
#define SCANCODE_CAPS   0x58

#define BREAK_CODE      0xF0

void initKeyboard (IN bool verbose);
int8 readKey();
void readLine (OUT cstr outStr);
void processScancode ();

typedef struct KeypressInfo {
    uint8 scancode;
    // RELEASED, ALT, CTRL, SHIFT, CAPS
    uint8 flags;
} KeypressInfo;

__attribute__((interrupt))
void IRQ1_keyboardHandler (IN InterruptFrame* frame);