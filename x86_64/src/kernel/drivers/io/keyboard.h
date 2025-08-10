// http://www.quadibloc.com/comp/scan.htm

#pragma once

#include <kernel/interrupts/idt.h>

#include <utils/types.h>

#define INPUT_BUFFER_SIZE    10

#define SCANCODE_INVALID     0x0
#define SCANCODE_ALT         0x11
#define SCANCODE_CTRL        0x14
#define SCANCODE_SHIFT       0x12
#define SCANCODE_CAPS        0x58
#define SCANCODE_ENTER       0x5A

#define SCANCODE_ARROW_UP    0x75
#define SCANCODE_ARROW_DOWN  0x72
#define SCANCODE_ARROW_LEFT  0x6B
#define SCANCODE_ARROW_RIGHT 0x74

#define BREAK_CODE           0xF0
#define EXTENDED_CODE        0xE0

namespace Drivers::Keyboard {
typedef struct KeypressInfo {
    uint8_t scancode;
    uint8_t keycode;
    // EXTENDED, CAPS, SHIFT, CTRL, ALT, PRESSED
    uint8_t flags;

} KeypressInfo;

enum KeypressInfoFlags { KEYDOWN = 0x1, ALT = 0x2, CTRL = 0x4, SHIFT = 0x8, CAPS = 0x10, EXTENDED = 0x20 };

void    init();
uint8_t get_keycode(IN uint8_t scancode, IN bool extended);
void    process_scancode();

KeypressInfo read_key();
void         read_line(OUT KeypressInfo *buffer);
} // namespace Drivers::Keyboard

//__attribute__((interrupt)) void IRQ1_keyboard_handler(IN Interrupts::IDT::InterruptFrame *frame);
