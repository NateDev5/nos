#include <kernel/library/log.h>
#include <kernel/library/panic.h>

#include <kernel/drivers/video/vga.h>

#include <utils/asm.h>

namespace Kernel {
void panic(IN CONST_CHAR_PTR msg) {
    error_screen(msg);

    cli();
    while (true)
        hlt();
}

void error_screen(IN CONST_CHAR_PTR error_msg) {
    Drivers::VGA::disable_cursor();
    // Library::clear();
    Library::print("ERROR: ");
    Library::println(error_msg);
    Drivers::VGA::set_background_color(Drivers::VGA::LRED);
    Drivers::VGA::set_foreground_color(Drivers::VGA::BLACK);
}
} // namespace Kernel
