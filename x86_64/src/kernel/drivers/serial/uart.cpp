#include <kernel/drivers/serial/uart.h>
#include <kernel/drivers/video/vga.h>

#include <utils/asm.h>
#include <utils/types.h>

#include <kernel/library/log.h>

namespace Drivers::UART {
void init_port(IN uint16_t port) {
    outb(port + 1, 0x00); // disable interrupts

    outb(port + 3, 0x80); // enable the dlab (port+3 is Line control register)

    outb(port + 0, 0x01); // set lest significant byte of divisor
    outb(port + 1, 0x00); // set most significant byte of divisor

    outb(port + 3, 0x00); // clear the dlab

    outb(port + 3, 0x06); // set the mode (no parity, 7 bits of data
                          // transmitted, 1 stop bit)
}
} // namespace Drivers::UART
