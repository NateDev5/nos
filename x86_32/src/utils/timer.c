#include "timer.h"
#include "asm.h"
#include "math.h"
#include "../video/video.h"
#include "../interrupts/pic.h"

static uint32 count;

// power hungry
void sleepFor50000NOP(IN uint16 numberOfCycles)
{
    for (uint32 cycleCount = 0; cycleCount < numberOfCycles * 50000; cycleCount++)
        nop();
}

void sleepPIT(IN uint16 milliseconds)
{
    count = milliseconds;

    while (count > 0)
        hlt();
}

void timerTick (IN InterruptFrame* frame) {
    if(count > 0) count--;
    sendEOI(0);
}