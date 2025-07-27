#include <kernel/terminal.h>

#include <kernel/library/arrayutils.h>
#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/string.h>

#include <kernel/drivers/io/keycodes.h>
#include <kernel/drivers/io/keymap.h>
#include <kernel/drivers/video/vga.h>
#include <kernel/drivers/hardware/pit.h>

#include <kernel/memory/mem.h>

namespace Kernel::Terminal {
static constexpr char character_map_caps[] = {
    0,   'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
};

static constexpr char character_map[][KEYCODE_MAP_LEN] = {
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
    {0,
     // a-z
     'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
     'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',

     // 0-9
     ')', '!', '@', '#', '$', '%', '^', '&', '*', '(',

     // special characters
     '~', '_', '+', '{', '}', '|', ':', '"', '<', '>', '?',

     // f keys
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

     // other
     0 /* back space */, ' ' /* space */, 0 /* tab */, 0, 0, 0, 0,
     0 /* enter */, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

static uint16_t current_offset = 0;
static uint16_t min_offset = 0;
static uint16_t max_offset = 0;

static char    terminal_buffer[TERMINAL_BUFFER_SIZE];
static uint8_t terminal_buffer_pos = 0;

void run() {
    Drivers::VGA::enable_cursor(Drivers::VGA::UNDERSCORE);

    new_entry();

    while (true) {
        Drivers::Keyboard::KeypressInfo keypress =
            Drivers::Keyboard::read_key();
        handle_keypress(keypress);
        Drivers::PIT::sleep(1);
    }
}

void handle_keypress(IN Drivers::Keyboard::KeypressInfo keypress) {
    if (keypress.flags & Drivers::Keyboard::KEYDOWN &&
        keypress.scancode != SCANCODE_INVALID) {
        if (keypress.keycode == KEYCODE_ENTER) {
            Library::print("\n");
            handle_buffer();
            new_entry();
            return;
        }

        if (keypress.keycode == KEYCODE_BACKSPACE) {
            if (terminal_buffer_pos == 0)
                return;

            // - 2 because at the start of every entry there is "> "
            uint16_t cur_cursor_pos =
                current_offset % Drivers::VGA::get_screen_width() - 2;

            if (cur_cursor_pos == 0)
                return;

            terminal_buffer_pos =
                Library::remove_at<char>(terminal_buffer, TERMINAL_BUFFER_SIZE,
                                   terminal_buffer_pos, cur_cursor_pos - 1);

            current_offset--;

            Drivers::VGA::remove_char_at(current_offset * 2);

            Drivers::VGA::set_cursor_offset(current_offset);
            return;
        }

        if (keypress.flags & Drivers::Keyboard::EXTENDED &&
            keypress.scancode == SCANCODE_ARROW_LEFT &&
            current_offset > min_offset) {
            current_offset--;
            Drivers::VGA::set_cursor_offset(current_offset);
            return;
        }

        if (keypress.flags & Drivers::Keyboard::EXTENDED &&
            keypress.scancode == SCANCODE_ARROW_RIGHT &&
            current_offset < max_offset) {
            current_offset++;
            Drivers::VGA::set_cursor_offset(current_offset);
            return;
        }

        if (keypress.keycode != KEYCODE_INVALID) {
            if (keypress.flags & Drivers::Keyboard::CAPS &&
                keypress.keycode >= KEYCODE_A &&
                keypress.keycode <= KEYCODE_Z) {
                display_character(character_map_caps[keypress.keycode]);
                return;
            }

            uint8_t tableIndex =
                keypress.flags & Drivers::Keyboard::SHIFT ? 1 : 0;
            int8_t _char = character_map[tableIndex][keypress.keycode];
            display_character(_char);
        }
    }
}

void display_character(IN char _char) {
    if (terminal_buffer_pos == TERMINAL_BUFFER_SIZE)
        return;

    uint16_t cur_cursor_pos =
        current_offset % Drivers::VGA::get_screen_width() - 2;
   
    Drivers::VGA::f_insert_char_at(_char, current_offset * 2);

    terminal_buffer_pos =
        Library::add_at<char>(terminal_buffer, TERMINAL_BUFFER_SIZE,
                              terminal_buffer_pos, cur_cursor_pos, _char);

    current_offset++;
    max_offset++;
    Drivers::VGA::set_cursor_offset(current_offset);
}

void new_entry() {
    Library::print("> ");
    current_offset = Drivers::VGA::get_current_offset() / 2;
    min_offset = current_offset;
    max_offset = current_offset;
    Drivers::VGA::set_cursor_offset(current_offset);
}

void handle_buffer() {
    if (Library::strcmp(terminal_buffer, "clear") == 0)
        Library::clear();
    else if (Library::strcmp(terminal_buffer, "vgatest") == 0)
        Drivers::VGA::test();
    else if (Library::strcmp(terminal_buffer, "nos") == 0) {
        Library::fprintln(
            " /$$   /$$  /$$$$$$   /$$$$$$\n| $$$ | $$ /$$__  $$ /$$__  $$\n| "
            "$$$$| $$| $$  \\ $$| $$  \\__/\n| $$ $$ $$| $$  | $$|  $$$$$$ \n| "
            "$$  $$$$| $$  | $$ \\____  $$\n| $$\\  $$$| $$  | $$ /$$  \\ "
            "$$\n| $$ \\  $$|  $$$$$$/|  $$$$$$/\n|__/  \\__/ \\______/  "
            "\\______/",
            Drivers::VGA::CYAN);
    } else
        Library::fprintf_ln("'%s' is not a valid command", Drivers::VGA::LRED,
                            terminal_buffer);

    Memory::memset((PTRMEM)terminal_buffer, 0, TERMINAL_BUFFER_SIZE);
    terminal_buffer_pos = 0;
}
} // namespace Kernel::Terminal
