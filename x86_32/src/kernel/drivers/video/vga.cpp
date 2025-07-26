#include "utils/types.h"
#include <kernel/drivers/video/vga.h>
#include <kernel/memory/mem.h>

#include <kernel/library/arrayutils.h>
#include <kernel/library/debug.h>
#include <kernel/library/panic.h>
#include <kernel/library/stdarg.h>
#include <kernel/library/string.h>

#include <kernel/library/log.h>

#include <utils/asm.h>
#include <utils/math.h>

namespace Drivers::VGA {
static uint16 current_offset = 0;

void init() {
    // Memory::memset((PTRMEM)START_VID_MEM, 0, END_VID_MEM - START_VID_MEM);
}

void modify_register(uint8 reg, uint8 data) {
    outb(ADDRESS_REGISTER_PORT, reg);
    outb(DATA_REGISTER_PORT, data);
}

void enable_cursor(IN CursorStyle cursor_style) {
    uint8 max_scan_lines = inb(MAX_SCAN_LINE_REGISTER);
    max_scan_lines &= FIRST_5_BIT_MASK;
    switch (cursor_style) {
    case UNDER:
        modify_register(CURSOR_START_REGISTER, 0xD);
        modify_register(CURSOR_END_REGISTER, max_scan_lines);
        break;
    case LARGE:
        modify_register(CURSOR_START_REGISTER, 0);
        modify_register(CURSOR_END_REGISTER, max_scan_lines);
    }
}

void disable_cursor() {
    modify_register(CURSOR_START_REGISTER, CURSOR_DISABLED); // 0010 0000
}

void set_cursor_pos(IN uint16 offset) {
    modify_register(CURSOR_LOCATION_HIGH_REGISTER, (offset >> 8) & 0xFF);
    modify_register(CURSOR_LOCATION_LOW_REGISTER, offset & 0xFF);
}

void fputchar(IN int8 _char, IN uint8 format) {
    if (_char == NULL)
        Kernel::panic("(putcharf) _char is null");

    if (current_offset >= SCRN_SIZE * 2)
        scroll_up();

    if (_char == '\n') {
        uint16 cur_line = (current_offset / 2) / SCRN_WIDTH + 1;
        if(cur_line == SCRN_HEIGHT) {
            scroll_up();
            return;
        }
        uint32 offset_for_new_line = current_offset % (SCRN_WIDTH * 2);
        current_offset += (SCRN_WIDTH * 2) - offset_for_new_line;
        return;
    }

    uint8 *video_mem = (uint8 *)BASE_VID_MEM;
    video_mem[current_offset] = _char;
    video_mem[current_offset + 1] = format;
    current_offset += 2;

    // setCursorPos(current_offset / 2);
}

void putchar(IN int8 _char) { fputchar(_char, BWHITE); }

void fputstr(IN cstr string, IN uint8 format) {
    if (string == NULL)
        Kernel::panic("(fputstr) string is null");

    for (uint32 i = 0; (string[i] != 0) /*&& (i < SCRN_SIZE)*/; i++)
        fputchar(string[i], format);
}
void putstr(IN cstr string) { fputstr(string, BWHITE); }

void fputchar_at(IN int8 _char, IN uint16 offset) {
    if (offset > SCRN_SIZE * 2) {
        Debug::fprintln("(ERROR) fputchar_at: offset is bigger than screen size (Offset : %i)", offset);
        return;
    }

    uint8 *video_mem = (uint8 *)BASE_VID_MEM;
   
    for (uint16 i = current_offset; i > offset - 2; i -= 2) {
        video_mem[i + 2] = video_mem[i];
        video_mem[i + 3] = video_mem[i + 1];
    }

    video_mem[offset] = _char;
    video_mem[offset + 1] = BASE_FMT;
    
    current_offset += 2;
}

void popchar() {
    uint8 *video_mem = (uint8 *)BASE_VID_MEM;
    video_mem[current_offset - 1] = 0;
    video_mem[current_offset - 2] = 0;
    current_offset -= 2;
}

void remove_char_at(IN uint16 offset) {
    if (offset > SCRN_SIZE * 2) {
        Debug::fprintln("(ERROR) removechar_at: offset is bigger than screen size (Offset : %i)", offset);
        return;
    }

    uint8 *video_mem = (uint8 *)BASE_VID_MEM;

    for (uint16 i = offset; i < current_offset; i += 2) {
        video_mem[i] = video_mem[i + 2];
        video_mem[i + 1] = video_mem[i + 3];

        video_mem[i + 2] = NULL;
        video_mem[i + 3] = BASE_FMT;
    }

    current_offset -= 2;
}

void set_background_color(IN uint8 color) {
    uint8 *video_mem = (uint8 *)BASE_VID_MEM;

    for (int32 i = 0; i <= SCRN_SIZE; i++) {
        // 0x0F is a mask for 00001111 so applying a logical AND will get only
        // the four bytes
        int32 cur_fore_color = video_mem[i * 2 + 1] & 0x0F;
        video_mem[i * 2 + 1] = color << 4 | cur_fore_color;
    }
}

void set_foreground_color(IN uint8 color) {
    uint8 *video_mem = (uint8 *)BASE_VID_MEM;
    for (int32 i = 0; i <= SCRN_SIZE; i++) {
        // 0x0F is a mask for 11110000 so applying a logical AND will get only
        // the four bytes
        int32 cur_back_color = video_mem[i * 2 + 1] & 0xF0;
        video_mem[i * 2 + 1] = cur_back_color | color;
    }
}

void clear_screen() {
    current_offset = 0;

    uint8 *video_mem = (uint8 *)BASE_VID_MEM;

    for (uint32 ui = 0; ui < SCRN_SIZE * 2; ui++) {
        if (ui % 2)
            video_mem[ui] = BASE_FMT;
        else
            video_mem[ui] = 0;
    }
}

void test() {
    uint8 *video_mem = (uint8 *)BASE_VID_MEM;
    for (int32 i = 0; i <= SCRN_SIZE; i++) {
        video_mem[i * 2] = 'A' + i;
        video_mem[i * 2 + 1] = i;
    }
}

void scroll_up() {
    uint16 buf_len = SCRN_SIZE * 2;
    uint8 cur_vid_buf[buf_len];
    
    Memory::memcpy((PTRMEM)(BASE_VID_MEM + (SCRN_WIDTH*2)), (PTRMEM)&cur_vid_buf[0], buf_len);
    
    Memory::memcpy((PTRMEM)&cur_vid_buf[0], (PTRMEM)BASE_VID_MEM, buf_len);

    current_offset = (SCRN_SIZE - SCRN_WIDTH) * 2;
}

uint16 get_current_offset() { return current_offset; }
} // namespace Drivers::VGA
