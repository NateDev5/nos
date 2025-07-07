#pragma once

#include "types.h"
#include "../interrupts/idt.h"

#define PIT_CHANNEL_0             0x40
#define PIT_MODE_COMMAND_REGISTER 0x43


void sleepFor50000NOP (IN uint16 numberOfCycles);
void sleepPIT (IN uint16 milliseconds);
void sleep (IN uint16 milliseconds);

__attribute__((interrupt))
void timerTick (IN InterruptFrame* frame);