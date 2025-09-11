#include <kernel/terminal/shell.h>
#include <kernel/terminal/term.h>

#include <kernel/drivers/video/framebuffer.h>

#include <kernel/library/log.h>

namespace Kernel::Shell {
void run_shell() {
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
                new_entry();
                break;
            }
            case '\b': {
                break;
            }
            default:
                Library::printc(keypress.unicode);
                break;
            }
        }
    }
}

void new_entry() { Library::print("> "); }
} // namespace Kernel::Shell
