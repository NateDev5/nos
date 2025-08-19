#include <kernel/drivers/video/video.h>

#include <kernel/library/debug.h>
#include <kernel/library/panic.h>

#include <boot/limine/limine.h>

namespace Drivers::Video {

__attribute__((used, section(".limine_requests"))) volatile limine_framebuffer_request framebuffer_request = {.id       = LIMINE_FRAMEBUFFER_REQUEST,
                                                                                                              .revision = 0,
                                                                                                              .response = nullptr};
limine_framebuffer                                                                    *framebuffer;

FramebufferInfo fb_info;

void init() {
    if (framebuffer_request.response == NULLPTR || framebuffer_request.response->framebuffer_count < 1)
        Kernel::panic("No usable framebuffer found");

    framebuffer = framebuffer_request.response->framebuffers[0];

    fb_info.width  = framebuffer->width;
    fb_info.height = framebuffer->height;

    fb_info.red_mask_shift = framebuffer->red_mask_shift;
    fb_info.red_mask_size  = framebuffer->red_mask_size;

    fb_info.green_mask_shift = framebuffer->green_mask_shift;
    fb_info.green_mask_size  = framebuffer->green_mask_size;

    fb_info.blue_mask_shift = framebuffer->blue_mask_shift;
    fb_info.blue_mask_size  = framebuffer->blue_mask_size;

    fb_info.pitch = framebuffer->pitch / sizeof(uint32_t);
    fb_info.bpp   = framebuffer->bpp;

    fb_info.pixel_width = fb_info.bpp / 8 / sizeof(uint32_t);

    fb_info.address = (uint64_t)framebuffer->address;

    DEBUG_PRINT("\nFramebuffer fb_info:")
    DEBUG_PRINT("   - Size: %ix%i", fb_info.width, fb_info.height)
    DEBUG_PRINT("   - Addr: %H", (uint64_t *)fb_info.address)
    DEBUG_PRINT("   - Pitch: %i", fb_info.pitch)
    DEBUG_PRINT("   - Bits per pixel: %i", fb_info.bpp)
    DEBUG_PRINT("   - Pixel width: %i", fb_info.pixel_width)
    DEBUG_PRINT("   - Color model: %h", framebuffer->memory_model)
}

void put_pixel(IN uint64_t x, IN uint64_t y, IN uint32_t color) {
    if(x >= (fb_info.width / fb_info.pixel_width) || y >= (fb_info.height / fb_info.pixel_width)) {
        DEBUG_PRINT("(ERROR) Trying to plot pixel out of bounds (x: %i, y: %i)", x, y)
        return;
    }

    uint32_t *fb        = (uint32_t *)fb_info.address;
    uint32_t  pixel_loc = (x * fb_info.pixel_width) + (y * fb_info.pitch);
    fb[pixel_loc]       = color;
}

uint64_t width() { return fb_info.width / fb_info.pixel_width; }
uint64_t height() { return fb_info.height / fb_info.pixel_width; }
} // namespace Drivers::Video
