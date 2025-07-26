#include <kernel/drivers/hardware/pit.h>
#include <kernel/drivers/video/vga.h>

#include <kernel/library/log.h>
#include <kernel/library/debug.h>

#include <kernel/interrupts/pic.h>

#include <utils/asm.h>
#include <utils/math.h>

namespace Drivers::PIT {
void init() {
    // CNTR = 0, RW = 3, Mode = 2, BCD = 0
    // In binary : 0|011|001|0 or 0x32
    outb(PIT_COMMAND_REGISTER,
         0x36); // send command byte to set the mode and such of the PIT

    // this will generate an interrupt every 0.001 second aka 1 millisecond
    uint16 divisor = 1193182 / 1000;
    outb(PIT_CHANNEL_0, divisor & 0xFF);
    outb(PIT_CHANNEL_0, divisor >> 8);

    Debug::fprintln("(OK) PIT initialized");
}

static uint32 cur_milliseconds;

void sleep(IN uint32 milliseconds) {
    cur_milliseconds = milliseconds;

    while (cur_milliseconds > 0)
        hlt();
}
} // namespace Drivers::PIT

void IRQ0_timer_handler(IN Interrupts::IDT::InterruptFrame *frame) {
    if (Drivers::PIT::cur_milliseconds > 0)
        Drivers::PIT::cur_milliseconds--;

    Interrupts::PIC::send_EOI(0);
}
