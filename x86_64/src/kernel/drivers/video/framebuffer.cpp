#include <kernel/drivers/video/framebuffer.h>
#include <utils/types.h>

#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/panic.h>

#include <kernel/memory/mem.h>

#include <deps/font8x8.h>

#include <boot/limine/limine.h>

namespace Drivers::Video::Framebuffer {

__attribute__((used, section(".limine_requests"))) volatile limine_framebuffer_request framebuffer_request = {.id       = LIMINE_FRAMEBUFFER_REQUEST,
                                                                                                              .revision = 0,
                                                                                                              .response = nullptr};
FramebufferInfo fb_info;

void init() {
    if (framebuffer_request.response == NULLPTR || framebuffer_request.response->framebuffer_count < 1)
        Kernel::panic("No usable framebuffer found");

    limine_framebuffer* framebuffer = framebuffer_request.response->framebuffers[0];

    fb_info.raw_width  = framebuffer->width;
    fb_info.raw_height = framebuffer->height;

    fb_info.red_mask_shift = framebuffer->red_mask_shift;
    fb_info.red_mask_size  = framebuffer->red_mask_size;

    fb_info.green_mask_shift = framebuffer->green_mask_shift;
    fb_info.green_mask_size  = framebuffer->green_mask_size;

    fb_info.blue_mask_shift = framebuffer->blue_mask_shift;
    fb_info.blue_mask_size  = framebuffer->blue_mask_size;

    fb_info.pitch = framebuffer->pitch / sizeof(uint32_t);
    fb_info.bpp   = framebuffer->bpp;

    fb_info.pixel_width = fb_info.bpp / 8 / sizeof(uint32_t);
    fb_info.ptr         = (uint32_t *)framebuffer->address;

    fb_info.width  = fb_info.raw_width / fb_info.pixel_width;
    fb_info.height = fb_info.raw_height / fb_info.pixel_width;

    fb_info.font_width  = 8;
    fb_info.font_height = 8;

    DEBUG_PRINT("\nFramebuffer info:")
    DEBUG_PRINT("   - Size: %ix%i", fb_info.raw_width, fb_info.raw_height)
    DEBUG_PRINT("   - Addr: %H", fb_info.ptr)
    DEBUG_PRINT("   - Pitch: %i", fb_info.pitch)
    DEBUG_PRINT("   - Memory model: %i", framebuffer->memory_model)
    DEBUG_PRINT("   - Bits per pixel: %i", fb_info.bpp)
    DEBUG_PRINT("   - Pixel width: %i", fb_info.pixel_width)
    DEBUG_PRINT("   - Color model: %h", framebuffer->memory_model)
}

void draw_pixel(IN Point point, IN uint32_t color) {
    if (point.x >= fb_info.width || point.y >= fb_info.height) {
        DEBUG_PRINT("(ERROR) Trying to plot pixel out of bounds (x: %i, y: %i)", point.x, point.y)
        return;
    }

    // uint32_t *fb        = (uint32_t *)fb_info.address;
    uint32_t pixel_loc     = (point.x * fb_info.pixel_width) + (point.y * fb_info.pitch);
    fb_info.ptr[pixel_loc] = color;
}

void draw_char(IN Point point, IN char _char, IN uint32_t color) {
    point.x *= fb_info.font_width;
    point.y *= fb_info.font_height;
    if (point.x > fb_info.width || point.y > fb_info.height) {
        DEBUG_PRINT("(ERROR) Trying to draw character out of bounds (x: %i, y: %i)", point.x, point.y)
        return;
    }

    uint8_t *glyph = font8x8_basic[(uint8_t)_char];

    for (uint8_t row = 0; row < fb_info.font_width; row++) {
        for (uint8_t col = 0; col < fb_info.font_height; col++) {
            uint8_t bit = glyph[row] & 1 << col;
            // * 8 since font is 8 pixel by 8
            // if(bit) draw_pixel({col + point.x, row + point.y}, color);
            draw_pixel({col + point.x, row + point.y}, bit ? color : 0);
        }
    }
}

void clear() {
    uint64_t size = (fb_info.raw_width * fb_info.raw_height) * 4; // multiply by 4 because each color is 4 bytes and memset does byte by byte
    Memory::memset(fb_info.ptr, 0, size);
}

uint64_t width() { return fb_info.width; }

uint64_t height() { return fb_info.height; }
} // namespace Drivers::Video::Framebuffer
