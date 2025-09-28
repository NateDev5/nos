#include "utils/types.h"
#include <kernel/terminal/term.h>

#include <kernel/library/debug.h>
#include <kernel/memory/mem.h>

#include <kernel/drivers/video/framebuffer.h>

namespace Kernel::Terminal {
static TerminalCell term_buffer_grid[TEMP_TERM_HEIGHT][TEMP_TERM_WIDTH];
static uint8_t      term_buffer_lines[TEMP_TERM_HEIGHT];

TerminalInfo info;

void init() {
    TODO("Add ANSI support")
    TODO("Render characters more efficently")
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
    case '\b':
        modify_cell(--info.x, info.y, 0);
        break;
    default:
        modify_cell(info.x++, info.y, _char);
        break;
    }

    if (info.x >= info.width) {
        info.x = 0;
        info.y++;
    }

    if (info.y >= info.height) {
        info.x = 0;
        info.y--;
        scroll_up();
    }
}

void print(IN CONST_CHAR_PTR str) {
    for (uint32_t i = 0; str[i] != '\0'; i++)
        print_char(str[i]);
}

void modify_cell(IN uint32_t x, IN uint32_t y, IN char unicode) {
    term_buffer_grid[y][x] = { unicode, DIRTY};
    term_buffer_lines[y] = DIRTY;
    render_term();
}

void render_term() {
    for (uint32_t y = 0; y < TEMP_TERM_HEIGHT; y++) {
        if (term_buffer_lines[y] == DIRTY) {
            for (uint32_t x = 0; x < TEMP_TERM_WIDTH; x++) {
                TerminalCell *cell = &term_buffer_grid[y][x];
                if (cell->dirty) {
                    Drivers::Video::Framebuffer::draw_char({x, y}, cell->unicode, 0xFFFFFFFF);
                    cell->dirty = NOT_DIRTY;
                }
            }
            term_buffer_lines[y] = NOT_DIRTY;
        }
    }
}

void scroll_up () {
    for(uint32_t y = 1; y < TEMP_TERM_HEIGHT; y++) {
        Memory::memcpy((PTRMEM)&term_buffer_grid[y - 1], (PTRMEM)&term_buffer_grid[y], TEMP_TERM_WIDTH, true);
        term_buffer_lines[y] = DIRTY;
        // term_buffer_grid[y - 1] = term_buffer_grid[y];
    }
    render_term();
}

void clear() {
    Drivers::Video::Framebuffer::clear();

    TODO("Use memset was broken at the time")
    for (uint32_t y = 0; y < TEMP_TERM_HEIGHT; y++) {
        for (uint32_t x = 0; x < TEMP_TERM_WIDTH; x++) {
            term_buffer_grid[y][x] = {0, NOT_DIRTY};
        }
        term_buffer_lines[y] = NOT_DIRTY;
    }

    info.x = 0;
    info.y = 0;
}
} // namespace Kernel::Terminal
