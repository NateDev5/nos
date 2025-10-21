#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/panic.h>

#include <kernel/drivers/video/vga.h>

#include <utils/asm.h>

namespace Kernel {
void panic(IN CONST_CHAR_PTR msg) {
    DEBUG_FATAL("(PANIC) %s", msg);
    Library::printf_ln("(!!!) Kernel Panic: %s", msg);

    cli();
    while (true)
        hlt();
}
} // namespace Kernel
