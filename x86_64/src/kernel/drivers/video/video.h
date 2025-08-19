#pragma once

#include <utils/types.h>

namespace Drivers::Video {
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

void init();

void put_pixel(IN uint64_t x, IN uint64_t y, IN uint32_t color);

uint64_t width();
uint64_t height();
} // namespace Drivers::Video
