#pragma once

#include <utils/types.h>

#define WHITE 0xFFFFFFFF

namespace Drivers::Video::Framebuffer {
struct FramebufferInfo {
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

    uint64_t address;
};

struct Point {
    uint32_t x;
    uint32_t y;
};

void init();

void draw_pixel(IN Point point, IN uint32_t color);
void draw_char (IN Point point, IN char _char, IN uint32_t color);
void clear();

uint64_t width();
uint64_t height();
} // namespace Drivers::Video
