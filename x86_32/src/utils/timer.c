#include "timer.h"

// power hungry
void sleepFor50000NOP(IN uint16 numberOfCycles)
{
    for (uint32 cycleCount = 0; cycleCount < numberOfCycles * 50000; cycleCount++)
        nop();
}