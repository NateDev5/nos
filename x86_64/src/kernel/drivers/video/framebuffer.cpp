#include <kernel/drivers/video/framebuffer.h>
#include <utils/types.h>

#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/panic.h>

#include <shared/mem.h>

#include <deps/font8x8.h>

#include <boot/limine/limine.h>

namespace Drivers::Video::Framebuffer {

__attribute__((used, section(".limine_requests"))) volatile limine_framebuffer_request framebuffer_request = {.id       = LIMINE_FRAMEBUFFER_REQUEST,
                                                                                                              .revision = 0,
                                                                                                              .response = nullptr};
FramebufferInfo                                                                        fb_info;

void init() {
    if (framebuffer_request.response == NULLPTR || framebuffer_request.response->framebuffer_count < 1)
        Kernel::panic("No usable framebuffer found");

    limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

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

    fb_info.char_padding = {0, 2};

    fb_info.font_size_raw = {8, 8};
    fb_info.font_size = {
        fb_info.font_size_raw.x + (fb_info.char_padding.x * 2),
        fb_info.font_size_raw.y + (fb_info.char_padding.y * 2)
    };

    DEBUG_INFO("Framebuffer info:")
    DEBUG_INFO("   - Size: %ix%i", fb_info.raw_width, fb_info.raw_height)
    DEBUG_INFO("   - Addr: %H", fb_info.ptr)
    DEBUG_INFO("   - Pitch: %i", fb_info.pitch)
    DEBUG_INFO("   - Memory model: %i", framebuffer->memory_model)
    DEBUG_INFO("   - Bits per pixel: %i", fb_info.bpp)
    DEBUG_INFO("   - Pixel width: %i", fb_info.pixel_width)
    DEBUG_INFO("   - Color model: %h", framebuffer->memory_model)
}

void draw_pixel(IN Shared::Point pos, IN uint32_t color) {
    if (pos.x >= fb_info.width || pos.y >= fb_info.height) {
        DEBUG_WARN("Trying to plot pixel out of bounds (x: %i, y: %i)", pos.x, pos.y)
        return;
    }

    // uint32_t *fb        = (uint32_t *)fb_info.address;
    uint32_t pixel_loc     = (pos.x * fb_info.pixel_width) + (pos.y * fb_info.pitch);
    fb_info.ptr[pixel_loc] = color;
}

void draw_char_no_padding(IN Shared::Point pos, IN char _char, IN uint32_t forecolor, IN uint32_t backcolor) {
    pos.x *= fb_info.font_size_raw.x;
    pos.y *= fb_info.font_size_raw.y;
    if (pos.x > fb_info.width || pos.y > fb_info.height) {
        DEBUG_WARN("Trying to draw character out of bounds (x: %i, y: %i)", pos.x, pos.y)
        return;
    }

    uint8_t *glyph = font8x8_basic[(uint8_t)_char];

    for (uint8_t row = 0; row < fb_info.font_size_raw.x; row++) {
        for (uint8_t col = 0; col < fb_info.font_size_raw.y; col++) {
            uint8_t bit = glyph[row] & 1 << col;
            // * 8 since font is 8 pixel by 8
            draw_pixel({col + pos.x, row + pos.y}, bit ? forecolor : backcolor);
        }
    }
}

void draw_char (IN Shared::Point pos, IN char _char, IN uint32_t forecolor, IN uint32_t backcolor) {
    pos.x *= fb_info.font_size.x;
    pos.y *= fb_info.font_size.y;
    if(pos.x > fb_info.width || pos.y > fb_info.height) {
        DEBUG_WARN("Trying to draw character out of bounds (x: %i, y: %i)", pos.x, pos.y);
        return;
    }

    uint8_t* glyph = font8x8_basic[(uint8_t)_char];

    // could make this faster by writing the bit from the font while drawing the rectangle
    // but works for now
    draw_rectangle(pos, fb_info.font_size, backcolor);

    for (uint8_t row = 0; row < fb_info.font_size_raw.x; row++) {
        for (uint8_t col = 0; col < fb_info.font_size_raw.y; col++) {
            uint8_t bit = glyph[row] & 1 << col;
            // * 8 since font is 8 pixel by 8
            draw_pixel({col + pos.x + fb_info.char_padding.x, row + pos.y + fb_info.char_padding.y}, bit ? forecolor : backcolor);
        }
    }
}

void draw_rectangle(IN Shared::Point pos, IN Shared::Point size, IN uint32_t color) {
    for(uint64_t x = 0; x < size.x; x++)
        for(uint64_t y = 0; y < size.y; y++)
            draw_pixel({x + pos.x, y + pos.y}, color);
}

void clear(IN uint32_t color) {
    uint64_t size = (fb_info.raw_width * fb_info.raw_height);// * 4; // multiply by 4 because each color is 4 bytes and memset does byte by byte
    Shared::memset32(fb_info.ptr, color, size);
}

uint64_t width() { return fb_info.width; }

uint64_t height() { return fb_info.height; }

Shared::Point font_size () { return fb_info.font_size; }
} // namespace Drivers::Video::Framebuffer
