#include <kernel/library/panic.h>
#include <kernel/library/log.h>

#include <kernel/drivers/video/vga.h>

#include <utils/asm.h>

namespace Kernel {
    void panic (IN cstr msg) {
        errorScreen(msg);

        cli();
        while (true)
            hlt();
    }

    void errorScreen (IN cstr errorMsg) {
        Library::clear();
        Library::print("ERROR: ");
        Library::println(errorMsg);
        Drivers::VGA::setBackgroundColor(Drivers::VGA::LRED);
        Drivers::VGA::setForegroundColor(Drivers::VGA::BLACK);
    }
}