#include <kernel/terminal/shell.h>
#include <kernel/terminal/term.h>

#include <kernel/library/log.h>

#include <kernel/drivers/io/keyboard.h>

namespace Kernel::Shell {
void run_shell () {
    Kernel::Terminal::clear();
    Library::println("Welcome to the NOS shell\n");
    Library::print("> ");

    while (true) {
        Drivers::Keyboard::KeypressInfo info = Drivers::Keyboard::read_key();
        if (info.unicode != 0 && info.flags & Drivers::Keyboard::KEYDOWN)
            Library::printc(info.unicode);
    }
}
}
