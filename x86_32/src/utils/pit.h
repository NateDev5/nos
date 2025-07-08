//http://www.osdever.net/bkerndev/Docs/pit.htm
//https://www.cpcwiki.eu/imgs/e/e3/8253.pdf

#pragma once

#include "types.h"
#include "../interrupts/idt.h"

#define PIT_CHANNEL_0             0x40
#define PIT_COMMAND_REGISTER      0x43

void initPIT (IN bool verbose);
void sleep (IN uint32 milliseconds);

__attribute__((interrupt))
void IRQ0_timerHandler (IN InterruptFrame* frame);