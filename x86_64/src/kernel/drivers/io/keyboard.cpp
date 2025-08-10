#include <kernel/drivers/io/keyboard.h>
#include <kernel/drivers/io/keymap.h>
#include <kernel/drivers/video/vga.h>

#include <kernel/interrupts/idt.h>
#include <kernel/interrupts/pic.h>

#include <kernel/library/debug.h>
#include <kernel/library/log.h>

#include <kernel/devices/io/ps2.h>

#include <utils/asm.h>
#include <utils/math.h>

namespace Drivers::Keyboard {
KeypressInfo input_buffer[INPUT_BUFFER_SIZE];
uint8_t      buffer_pos  = 0;
uint8_t      buffer_tail = 0;

uint8_t key_flags = 0x0;

bool next_interrupt_break_code    = false;
bool next_interrupt_extended_code = false;

void init() {
    Devices::PS2::init();
    DEBUG_PRINT("(OK) Keyboard initialized");
}

KeypressInfo read_key() {
    if (input_buffer[buffer_tail].scancode == SCANCODE_INVALID)
        return {};

    KeypressInfo info                  = input_buffer[buffer_tail];
    input_buffer[buffer_tail].scancode = SCANCODE_INVALID;
    input_buffer[buffer_tail].keycode  = KEYCODE_INVALID;
    input_buffer[buffer_tail].flags    = 0;

    buffer_tail = (buffer_tail + 1) % INPUT_BUFFER_SIZE;
    return info;
}

void read_line(OUT KeypressInfo *buffer) {}

uint8_t get_keycode(IN uint8_t scancode, IN bool extended) {
    auto map = extended ? keycode_map_extended : keycode_map;
    auto len = extended ? KEYCODE_MAP_EXTENDED_LEN : KEYCODE_MAP_LEN;

    for (uint8_t i = 0; i < len; i++) {
        if (map[i][0] == scancode)
            return map[i][1];
    }

    return KEYCODE_INVALID;
}

void process_scancode() {
    uint8_t flags    = 0x01 & !next_interrupt_break_code;
    uint8_t scancode = Devices::PS2::poll();

    if (scancode == EXTENDED_CODE) {
        next_interrupt_extended_code = true;
        return;
    }

    if (scancode == BREAK_CODE) {
        next_interrupt_break_code = true;
        return;
    }

    if (!next_interrupt_extended_code)
        switch (scancode) {
        case SCANCODE_ALT:
            key_flags = next_interrupt_break_code ? (key_flags & ~ALT) : (key_flags | ALT);
            break;
        case SCANCODE_CTRL:
            key_flags = next_interrupt_break_code ? (key_flags & ~CTRL) : (key_flags | CTRL);
            break;
        case SCANCODE_SHIFT:
            key_flags = next_interrupt_break_code ? (key_flags & ~SHIFT) : (key_flags | SHIFT);
            break;
        case SCANCODE_CAPS:
            if (next_interrupt_break_code)
                break;
            key_flags ^= CAPS;
            break;
        default:
            break;
        }

    flags |= key_flags;

    if (next_interrupt_extended_code)
        flags |= EXTENDED;

    input_buffer[buffer_pos].scancode = scancode;
    input_buffer[buffer_pos].flags    = flags;
    input_buffer[buffer_pos].keycode  = get_keycode(scancode, next_interrupt_extended_code);

    buffer_pos = (buffer_pos + 1) % INPUT_BUFFER_SIZE;

    next_interrupt_break_code    = false;
    next_interrupt_extended_code = false;
}
} // namespace Drivers::Keyboard

/*
void IRQ1_keyboard_handler(IN Interrupts::IDT::InterruptFrame *frame) {
    Drivers::Keyboard::process_scancode();
    Interrupts::PIC::send_EOI(1);
}
*/
