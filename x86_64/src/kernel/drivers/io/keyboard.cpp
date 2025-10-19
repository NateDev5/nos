#include <kernel/drivers/io/keyboard.h>
#include <kernel/drivers/io/keymap.h>
#include <kernel/drivers/video/vga.h>

#include <kernel/library/log.h>

#include <kernel/arch/x86_64/interrupts/idt.h>

#include <kernel/devices/io/ps2.h>

#include <utils/asm.h>

namespace Drivers::Keyboard {

static constexpr char character_map_caps[] = {
    0, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
};

static constexpr char character_map[][KEYCODE_MAP_LEN] = {
    {0,
     // a-z
     'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',

     // 0-9
     '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

     // special characters
     '`', '-', '=', '[', ']', '\\', ';', '\'', ',', '.', '/',

     // f keys
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

     // other
     '\b' /* back space */, ' ' /* space */, '\t' /* tab */, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\n' /* enter */, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0,
     // a-z
     'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',

     // 0-9
     ')', '!', '@', '#', '$', '%', '^', '&', '*', '(',

     // special characters
     '~', '_', '+', '{', '}', '|',  ':', '"',  '<', '>', '?',

     // f keys
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

     // other
     '\b' /* back space */, ' ' /* space */, '\t' /* tab */, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\n' /* enter */, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

KeypressInfo input_buffer[INPUT_BUFFER_SIZE];
uint8_t      buffer_pos  = 0;
uint8_t      buffer_tail = 0;

uint8_t key_flags = 0x0;

bool next_interrupt_break_code    = false;
bool next_interrupt_extended_code = false;

void init() {
    Devices::PS2::init();
    Arch::x86_64::IDT::set_irq_handler(1, (PTR)process_scancode);
    KLOG("(OK) Keyboard initialized");
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

char get_unicode(IN uint8_t keycode, IN uint8_t flags) {
    if (keycode != KEYCODE_INVALID) {
        if (flags & CAPS && keycode >= KEYCODE_A && keycode <= KEYCODE_Z)
            return character_map_caps[keycode];

        uint8_t tableIndex = flags & SHIFT ? 1 : 0;
        return character_map[tableIndex][keycode];
    }

    return 0;
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

    uint8_t keycode                  = get_keycode(scancode, next_interrupt_extended_code);
    input_buffer[buffer_pos].keycode = keycode;
    input_buffer[buffer_pos].unicode = get_unicode(keycode, flags);

    buffer_pos = (buffer_pos + 1) % INPUT_BUFFER_SIZE;

    next_interrupt_break_code    = false;
    next_interrupt_extended_code = false;
}
} // namespace Drivers::Keyboard
