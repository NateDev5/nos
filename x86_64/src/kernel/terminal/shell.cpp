#include <utils/types.h>

#include <kernel/terminal/shell.h>
#include <kernel/terminal/term.h>

#include <kernel/drivers/video/framebuffer.h>

#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/string.h>
#include <kernel/memory/mem.h>

namespace Kernel::Shell {

static char    buffer[KERNEL_SHELL_BUFFER_SIZE];
static uint8_t buffer_index = 0;

void run_shell() {
    TODO("Changed the TEMP_TERM_WIDTH/HEIGHT to dynamic size on the heap when memory allocator is done")
    Kernel::Terminal::clear();
    Library::println("Welcome to the NOS shell");
    new_entry();

    while (true) {
        Drivers::Keyboard::KeypressInfo info = Drivers::Keyboard::read_key();
        handle_keypress(info);
    }
}

void handle_keypress(IN Drivers::Keyboard::KeypressInfo keypress) {
    if (keypress.flags & Drivers::Keyboard::KEYDOWN && keypress.scancode != SCANCODE_INVALID) {
        if ((keypress.flags & Drivers::Keyboard::EXTENDED) == 0) {
            switch (keypress.unicode) {
            case '\n': {
                Library::printc('\n');
                handle_command();
                new_entry();
                break;
            }
            case '\b': {
                if (buffer_index != 0) {
                    buffer[--buffer_index] = 0;
                    Library::printc('\b');
                }
                break;
            }
            default:
                if (buffer_index != KERNEL_SHELL_BUFFER_SIZE) {
                    buffer[buffer_index++] = keypress.unicode;
                    Library::printc(keypress.unicode);
                }
                break;
            }
        }
    }
}

void handle_command() {
    TEMP

    if (Library::strcmp(buffer, "test") == 0)
        test_cmd();
    else if (Library::strcmp(buffer, "clear") == 0)
        Kernel::Terminal::clear();
}

void test_cmd() { TEMP Library::println("Hello world!"); }

void new_entry() {
    buffer_index = 0;

    TODO("Use memset, it was bugged at the time")
    for (uint64_t i = 0; i < KERNEL_SHELL_BUFFER_SIZE; i++)
        buffer[i] = 0;

    Library::print("> ");
}
} // namespace Kernel::Shell
