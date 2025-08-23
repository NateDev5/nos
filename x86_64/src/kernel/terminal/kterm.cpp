#include <kernel/terminal/kterm.h>

#include <kernel/library/debug.h>

#include <kernel/drivers/io/keyboard.h>
#include <kernel/drivers/video/framebuffer.h>

namespace Kernel::Terminal {
TerminalInfo info;

void run_kterm() {
    info.width  = Drivers::Video::Framebuffer::width() / 8;  // width in characters
    info.height = Drivers::Video::Framebuffer::height() / 8; // height in characters

    DEBUG_PRINT("\nTerminal info:")
    DEBUG_PRINT("   - Width: %i", info.width)
    DEBUG_PRINT("   - Height: %i", info.height)
    DEBUG_PRINT("   - Max characters: %i", info.width * info.height)

    while (true) {
        Drivers::Keyboard::KeypressInfo info = Drivers::Keyboard::read_key();
        // DEBUG_PRINT("%h", info.keycode)
        if (info.unicode != 0 && info.flags & Drivers::Keyboard::KEYDOWN)
            print_char(info.unicode);
    }
}

void print_char(IN char _char) {
    Drivers::Video::Framebuffer::draw_char({info.x, info.y}, _char, 0xFFFFFFFF);

    info.x++;
    if (info.x >= info.width) {
        info.x = 0;
        info.y++;
    }

    if (info.y >= info.height) {
        info.x = 0;
        info.y = 0;
    }
}

void print(IN CONST_CHAR_PTR str) {
    for (uint32_t i = 0; str[i] != '\0'; i++)
        print_char(str[i]);
}
} // namespace Kernel::Terminal
