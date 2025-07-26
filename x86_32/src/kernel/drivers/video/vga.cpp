#include <kernel/drivers/video/vga.h>
#include <kernel/memory/mem.h>

#include <kernel/library/arrayutils.h>
#include <kernel/library/debug.h>
#include <kernel/library/panic.h>
#include <kernel/library/string.h>

#include <kernel/library/log.h>

#include <utils/asm.h>
#include <utils/math.h>

#include <stdarg.h>

namespace Drivers::VGA {
static uint16_t current_offset = 0;

static uint16_t screen_width = TEXT_MODE_SCREEN_WIDTH;
static uint16_t screen_height = TEXT_MODE_SCREEN_HEIGHT;
static uint16_t screen_size = TEXT_MODE_SCREEN_HEIGHT;

void modify_register(uint8_t reg, uint8_t data) {
    outb(ADDRESS_REGISTER_PORT, reg);
    outb(DATA_REGISTER_PORT, data);
}

void enable_cursor(IN CursorStyle cursor_style) {
    uint8_t max_scan_lines = inb(MAX_SCAN_LINE_REGISTER);
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

void set_cursor_offset(IN uint16_t offset) {
    modify_register(CURSOR_LOCATION_HIGH_REGISTER, (offset >> 8) & 0xFF);
    modify_register(CURSOR_LOCATION_LOW_REGISTER, offset & 0xFF);
}

void set_cursor_pos(IN uint16_t x, IN uint16_t y) {
    set_cursor_offset(x * y);
}

void f_put_char(IN char _char, IN uint8_t color) {
    if (current_offset >= screen_size * 2)
        scroll_up();

    if (_char == '\n') {
        uint16_t cur_line = (current_offset / 2) / screen_width + 1;
        if (cur_line == screen_height) {
            scroll_up();
            return;
        }
        uint32_t offset_for_new_line = current_offset % (screen_width * 2);
        current_offset += (screen_width * 2) - offset_for_new_line;
        return;
    }

    uint8_t *video_mem = (uint8_t *)BASE_VID_MEM;
    video_mem[current_offset] = _char;
    video_mem[current_offset + 1] = color;
    current_offset += 2;

    // setCursorPos(current_offset / 2);
}

void put_char(IN char _char) { f_put_char(_char, BWHITE); }

void f_put_str(IN CONST_CHAR_PTR string, IN uint8_t color) {
    // assert(string != NULL, "(f_put_str) string is null");
    if (string == NULL)
        return;
    
    for (uint32_t i = 0; string[i] != 0; i++)
        f_put_char(string[i], color);
}
void put_str(IN CONST_CHAR_PTR string) { f_put_str(string, BWHITE); }

void f_insert_char_at(IN char _char, IN uint16_t offset) {
    if (offset > screen_size * 2) {
        Debug::fprintln("(ERROR) f_put_char_at: offset is bigger than screen "
                        "size (Offset : %i)",
                        offset);
        return;
    }

    uint8_t *video_mem = (uint8_t *)BASE_VID_MEM;

    for (uint16_t i = current_offset; i > offset - 2; i -= 2) {
        video_mem[i + 2] = video_mem[i];
        video_mem[i + 3] = video_mem[i + 1];
    }

    video_mem[offset] = _char;
    video_mem[offset + 1] = BASE_FMT;

    current_offset += 2;
}

void remove_char_at(IN uint16_t offset) {
    if (offset > screen_size * 2) {
        Debug::fprintln("(ERROR) removechar_at: offset is bigger than screen "
                        "size (Offset : %i)",
                        offset);
        return;
    }

    uint8_t *video_mem = (uint8_t *)BASE_VID_MEM;

    for (uint16_t i = offset; i < current_offset; i += 2) {
        video_mem[i] = video_mem[i + 2];
        video_mem[i + 1] = video_mem[i + 3];

        video_mem[i + 2] = NULL;
        video_mem[i + 3] = BASE_FMT;
    }

    current_offset -= 2;
}

void set_background_color(IN uint8_t color) {
    uint8_t *video_mem = (uint8_t *)BASE_VID_MEM;

    for (uint32_t i = 0; i <= screen_size; i++) {
        // 0x0F is a mask for 00001111 so applying a logical AND will get only
        // the four bytes
        int8_t cur_fore_color = video_mem[i * 2 + 1] & 0x0F;
        video_mem[i * 2 + 1] = color << 4 | cur_fore_color;
    }
}

void set_foreground_color(IN uint8_t color) {
    uint8_t *video_mem = (uint8_t *)BASE_VID_MEM;
    for (uint32_t i = 0; i <= screen_size; i++) {
        // 0x0F is a mask for 11110000 so applying a logical AND will get only
        // the four bytes
        int8_t cur_back_color = video_mem[i * 2 + 1] & 0xF0;
        video_mem[i * 2 + 1] = cur_back_color | color;
    }
}

void clear_screen() {
    current_offset = 0;

    uint8_t *video_mem = (uint8_t *)BASE_VID_MEM;

    for (uint32_t ui = 0; ui < screen_size * 2; ui++) {
        if (ui % 2)
            video_mem[ui] = BASE_FMT;
        else
            video_mem[ui] = 0;
    }
}

void test() {
    uint8_t *video_mem = (uint8_t *)BASE_VID_MEM;
    for (uint32_t i = 0; i <= screen_size; i++) {
        video_mem[i * 2] = 'A' + i;
        video_mem[i * 2 + 1] = i;
    }
}

void scroll_up() {
    uint16_t buf_len = screen_size * 2;
    uint8_t cur_vid_buf[buf_len];

    Memory::memcpy((PTRMEM)(BASE_VID_MEM + (screen_width * 2)),
                   (PTRMEM)&cur_vid_buf[0], buf_len);

    Memory::memcpy((PTRMEM)&cur_vid_buf[0], (PTRMEM)BASE_VID_MEM, buf_len);

    current_offset = (screen_size - screen_width) * 2;
}

uint16_t get_current_offset() { return current_offset; }
uint16_t get_screen_width() { return screen_width; }
uint16_t get_screen_height() { return screen_height; }
uint16_t get_screen_size() { return screen_size; }

} // namespace Drivers::VGA
