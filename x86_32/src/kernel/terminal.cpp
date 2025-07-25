#include <kernel/terminal.h>

#include <kernel/library/arrayutils.h>
#include <kernel/library/log.h>
#include <kernel/library/string.h>

#include <kernel/drivers/io/keycodes.h>
#include <kernel/drivers/io/keymap.h>
#include <kernel/drivers/video/vga.h>

#include <kernel/memory/mem.h>

namespace Kernel::Terminal {
static constexpr int8 character_map_caps[] = {
    0,   'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
};

static constexpr int8 character_map[][KEYCODE_MAP_LEN] = {
    {0,
     // a-z
     'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
     'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',

     // 0-9
     '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

     // special characters
     '`', '-', '=', '[', ']', '\\', ';', '\'', ',', '.', '/',

     // f keys
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

     // other
     0 /* back space */, ' ' /* space */, 0 /* tab */, 0, 0, 0, 0,
     0 /* enter */, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {
        0,
        // a-z
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
        'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',

        // 0-9
        ')', '!', '@', '#', '$', '%', '^', '&', '*', '(',

        // special characters
        '~', '_', '+', '{', '}', '|', ':', '"', '<', '>', '?',

        // f keys
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // other
        0 /* back space */, ' ' /* space */, 0 /* tab */, 0, 0, 0, 0,
        0 /* enter */, 0, 0, 0 //,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    }};

static uint16 currentOffset = 0;
static uint16 minOffset = 0;
static uint16 maxOffset = 0;

static int8 terminal_buffer[TERMINAL_BUFFER_SIZE];
static uint8 terminal_buffer_pos = 0;

void run() {
    Drivers::VGA::enable_cursor(Drivers::VGA::UNDER);
    new_entry();

    while (true) {
        Drivers::Keyboard::KeypressInfo keypress =
            Drivers::Keyboard::read_key();
        handle_keypress(keypress);
    }
}

void handle_keypress(IN Drivers::Keyboard::KeypressInfo keypress) {
    if (keypress.flags & Drivers::Keyboard::KEYDOWN &&
        keypress.scancode != SCANCODE_INVALID) {
        if (keypress.keycode == KEYCODE_ENTER) {
            // uint16 currentLine = (currentOffset / SCRN_WIDTH) + 1;
            Library::print("\n");
            handle_buffer();
            new_entry();
            return;
        }

        if (keypress.keycode == KEYCODE_BACKSPACE) {
            if (terminal_buffer_pos == 0)
                return;

            // - 2 because at the start of every entry there is "> "
            uint16 currentCursorPos = currentOffset % SCRN_WIDTH - 2;
            if (currentCursorPos == 0)
                return;
            terminal_buffer_pos =
                Library::remove_at(terminal_buffer, TERMINAL_BUFFER_SIZE,
                                   terminal_buffer_pos, currentCursorPos - 1);

            currentOffset--;

            Drivers::VGA::removechar_at(currentOffset * 2);

            Drivers::VGA::set_cursor_pos(currentOffset);
        }

        if (keypress.flags & Drivers::Keyboard::EXTENDED &&
            keypress.scancode == SCANCODE_ARROW_LEFT &&
            currentOffset > minOffset) {
            currentOffset--;
            Drivers::VGA::set_cursor_pos(currentOffset);
            return;
        }

        if (keypress.flags & Drivers::Keyboard::EXTENDED &&
            keypress.scancode == SCANCODE_ARROW_RIGHT &&
            currentOffset < maxOffset) {
            currentOffset++;
            Drivers::VGA::set_cursor_pos(currentOffset);
            return;
        }

        if (keypress.keycode != KEYCODE_INVALID) {
            if (keypress.flags & Drivers::Keyboard::CAPS &&
                keypress.keycode >= KEYCODE_A &&
                keypress.keycode <= KEYCODE_Z) {
                display_character(character_map_caps[keypress.keycode]);
                return;
            }

            uint8 tableIndex =
                keypress.flags & Drivers::Keyboard::SHIFT ? 1 : 0;
            int8 _char = character_map[tableIndex][keypress.keycode];
            display_character(_char);
        }
    }
}

void display_character(IN int8 _char) {
    if (_char == NULL)
        return;
    if (terminal_buffer_pos > TERMINAL_BUFFER_SIZE)
        return;

    Library::printc(_char);
    terminal_buffer[terminal_buffer_pos] = _char;
    terminal_buffer_pos++;

    currentOffset++;
    maxOffset++;
    Drivers::VGA::set_cursor_pos(currentOffset);
}

void new_entry() {
    Library::print("> ");

    currentOffset = Drivers::VGA::get_current_offset() / 2;
    minOffset = currentOffset;
    maxOffset = currentOffset;

    Drivers::VGA::set_cursor_pos(currentOffset);
}

void handle_buffer() {
    if (Library::strcmp(terminal_buffer, "clear"))
        Library::clear();
    else if (Library::strcmp(terminal_buffer, "vgatest"))
        Drivers::VGA::test();
    else
        Library::fprintf_ln("'%s' is not a valid command", Drivers::VGA::LRED,
                            terminal_buffer);

    Memory::memset(terminal_buffer, 0, TERMINAL_BUFFER_SIZE);
    terminal_buffer_pos = 0;
}
} // namespace Kernel::Terminal
