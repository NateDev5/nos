#pragma once

#include <kernel/interrupts/idt.h>

#include <utils/types.h>

#define INPUT_BUFFER_SIZE   256

#define SCANCODE_ALT        0x11
#define SCANCODE_CTRL       0x14
#define SCANCODE_SHIFT      0x12
#define SCANCODE_CAPS       0x58
#define SCANCODE_ENTER      0x5A

//         ↓    
// 0000 0001
#define ALT_FLAG            0x01

//        ↓    
// 0000 0010
#define CTRL_FLAG           0x02

//       ↓    
// 0000 0100
#define SHIFT_FLAG          0x04

//      ↓    
// 0000 1000
#define CAPS_FLAG           0x08

#define BREAK_CODE          0xF0

namespace Drivers::Keyboard {
    typedef struct KeypressInfo {
        uint8 scancode;
        // CAPS, SHIFT, CTRL, ALT, PRESSED
        uint8 flags;

    } KeypressInfo;

    void init (IN bool verbose);
    void processScancode ();

    int8 readKey();
    void readLine (OUT KeypressInfo* buffer);
}

__attribute__((interrupt))
void IRQ1_keyboardHandler (IN Interrupts::IDT::InterruptFrame* frame);