#pragma once

#include <utils/types.h>

#include <shared/utils.h>

namespace Drivers::Video::Framebuffer {
struct FramebufferInfo {
    uint64_t raw_width;
    uint64_t raw_height;

    uint64_t width;
    uint64_t height;

    uint8_t red_mask_shift;
    uint8_t red_mask_size;

    uint8_t green_mask_shift;
    uint8_t green_mask_size;

    uint8_t blue_mask_shift;
    uint8_t blue_mask_size;

    uint64_t pitch;
    uint16_t bpp;

    uint16_t pixel_width;

    uint32_t *ptr;

    Shared::Size font_size;
    Shared::Size font_size_raw;

    Shared::Point char_padding;
};

void init();

void     draw_pixel(IN Shared::Point pos, IN uint32_t color);
void     draw_char_no_padding(IN Shared::Point pos, IN char _char, IN uint32_t forecolor, IN uint32_t backcolor);
void     draw_char(IN Shared::Point pos, IN char _char, IN uint32_t forecolor, IN uint32_t backcolor);
void     draw_rectangle(IN Shared::Point pos, IN Shared::Point size, IN uint32_t color);
void     clear();
uint64_t width();
uint64_t height();
Shared::Point font_size();
} // namespace Drivers::Video::Framebuffer
