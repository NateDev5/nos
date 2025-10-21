#include <utils/types.h>

#include <kernel/terminal/shell.h>
#include <kernel/terminal/term.h>

#include <kernel/drivers/video/framebuffer.h>

#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/memory/pmm.h>

#include <shared/mem.h>
#include <shared/string.h>

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
    if (IS_CMD(buffer, "test"))
        test_cmd();
    else if (IS_CMD(buffer, "clear"))
        Kernel::Terminal::clear();
    else if (IS_CMD(buffer, "palloc"))
        palloc_cmd();
    else if (IS_CMD(buffer, "ansi"))
        ansi_test_cmd();
    else if (IS_CMD(buffer, "rect"))
        rect_cmd();
}

void new_entry() {
    buffer_index = 0;

    Shared::memset(buffer, 0, KERNEL_SHELL_BUFFER_SIZE);

    Library::print("> ");
}

void test_cmd() { TEMP Library::println("Hello world!"); }

void palloc_cmd() {
    TEMP

    Memory::Physical::alloc_page();
}

void ansi_test_cmd() { TEMP KLOG("\033[0;31;45mHello\033[0m"); }

void rect_cmd() {
    TEMP

        Drivers::Video::Framebuffer::draw_rectangle({0, 0}, {100, 100}, MAGENTA);
}
} // namespace Kernel::Shell
