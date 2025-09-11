#include <kernel/arch/x86_64/interrupts/idt.h>
#include <kernel/arch/x86_64/timer/pit.h>

#include <kernel/library/log.h>

#include <utils/asm.h>
#include <utils/math.h>

namespace Arch::x86_64::PIT {
void init() {
    // CNTR = 0, RW = 3, Mode = 2, BCD = 0
    // In binary : 0|011|001|0 or 0x32
    outb(PIT_COMMAND_REGISTER,
         0x36); // send command byte to set the mode and such of the PIT

    // this will generate an interrupt every 0.001 second aka 1 millisecond
    uint16_t divisor = 1193182 / 1000;
    outb(PIT_CHANNEL_0, divisor & 0xFF);
    outb(PIT_CHANNEL_0, divisor >> 8);

    IDT::set_irq_handler(0, (PTR)timer_handler);

    KLOG("(OK) PIT initialized");
}

static uint64_t cur_milliseconds;

void sleep(IN uint64_t milliseconds) {
    cur_milliseconds = milliseconds;

    while (cur_milliseconds > 0)
        hlt();
}

void timer_handler() {
    if (cur_milliseconds > 0)
        cur_milliseconds--;
}
} // namespace Arch::x86_64::PIT
