#include <kernel/terminal/term.h>

#include <kernel/library/debug.h>

#include <kernel/drivers/video/framebuffer.h>

namespace Kernel::Terminal {
TerminalInfo info;

void init() {
    info.width  = Drivers::Video::Framebuffer::width() / 8;  // width in characters
    info.height = Drivers::Video::Framebuffer::height() / 8; // height in characters

    KLOG("Terminal info:")
    KLOG("   - Width: %i", info.width)
    KLOG("   - Height: %i", info.height)
    KLOG("   - Max characters: %i\n", info.width * info.height)
}

void print_char(IN char _char) {
    switch (_char) {
    case '\n':
        info.x = 0;
        info.y++;
        break;
    default:
        Drivers::Video::Framebuffer::draw_char({info.x, info.y}, _char, 0xFFFFFFFF);
        info.x++;
        break;
    }

    if (info.x >= info.width) {
        info.x = 0;
        info.y++;
    }

    if (info.y >= info.height) {
        Drivers::Video::Framebuffer::scroll_up();
        info.x = 0;
        info.y -= 5;
    }
}

void print(IN CONST_CHAR_PTR str) {
    for (uint32_t i = 0; str[i] != '\0'; i++)
        print_char(str[i]);
}

void clear () {
    Drivers::Video::Framebuffer::clear();
    info.x = 0;
    info.y = 0;
}
} // namespace Kernel::Terminal
