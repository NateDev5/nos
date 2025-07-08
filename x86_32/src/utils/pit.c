#include "pit.h"
#include "asm.h"
#include "math.h"
#include "../video/vga.h"
#include "../interrupts/pic.h"

void initPIT(IN bool verbose)
{
    // CNTR = 0, RW = 3, Mode = 2, BCD = 0
    // In binary : 0|011|001|0 or 0x32
    outb(PIT_COMMAND_REGISTER, 0x36); // send command byte to set the mode and such of the PIT

    // this will generate an interrupt every 0.001 second aka 1 millisecond
    uint16 divisor = 1193182 / 1000;
    outb(PIT_CHANNEL_0, divisor & 0xFF);
    outb(PIT_CHANNEL_0, divisor >> 8);

    if(verbose) println("(OK) PIT initialized", LGREEN);
}

static uint32 curMilliseconds;

void sleep(IN uint32 milliseconds)
{
    curMilliseconds = milliseconds;

    while (curMilliseconds > 0)
        hlt();
}

void IRQ0_timerHandler(IN InterruptFrame *frame)
{
    if (curMilliseconds > 0)
            curMilliseconds--;

    sendEOI(0);
}