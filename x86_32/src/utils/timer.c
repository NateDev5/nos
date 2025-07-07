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

extern uint16 count;

void sleepPIT(IN uint16 milliseconds)
{
    count = milliseconds;
    printuint32(count, 10);

    while (count > 0)
        hlt();
}