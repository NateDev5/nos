#include <kernel/debug/assert.h>

#include <kernel/drivers/video/vga.h>

#include <utils/asm.h>

namespace Debug {
    void assert(IN bool expression, IN const cstr msg)
    {
        if (!expression)
        {
            Drivers::VGA::errorScreen(msg);
            cli();
            while (true)
                hlt();
        }
    }
}