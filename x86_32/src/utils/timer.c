#include "timer.h"
#include "asm.h"
#include "math.h"
#include "../video/video.h"

// power hungry
void sleepFor50000NOP(IN uint16 numberOfCycles)
{
    for (uint32 cycleCount = 0; cycleCount < numberOfCycles * 50000; cycleCount++)
        nop();
}
static uint32 curCount = 0;
void sleepPIT(IN uint16 milliseconds)
{
    curCount = milliseconds;
    outb(PIT_CHANNEL_0, 1 & 0xFF);
    outb(PIT_CHANNEL_0, (1 & 0xFF00) >> 8);

    uint16 count;
    do
    {
        outb(0x43, 0b0000000);

        count = inb(PIT_CHANNEL_0);
        count |= inb(PIT_CHANNEL_0) << 8;

        if(count < 1) curCount--;
    } while (curCount > 0);
}