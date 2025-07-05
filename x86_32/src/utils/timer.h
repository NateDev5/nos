#pragma once

#include "types.h"

static inline void nop() {
    __asm__ volatile ("nop");
}

static inline void hlt() {
    __asm__ volatile ("hlt");
}

void sleepFor50000NOP (IN uint16 numberOfCycles);
void sleepPIT (IN uint32 milliseconds);
void sleep (IN uint32 milliseconds);