// http://www.osdever.net/bkerndev/Docs/pit.htm
// https://www.cpcwiki.eu/imgs/e/e3/8253.pdf

#pragma once

#include <kernel/interrupts/idt.h>

#include <utils/types.h>

#define PIT_CHANNEL_0        0x40
#define PIT_COMMAND_REGISTER 0x43

namespace Drivers::PIT {
///
/// Setup the Programmable Interrupt Timer
/// Sets the mod and the inteval at which an interrupt is triggered
///
void init();
void sleep(IN uint32_t milliseconds);
} // namespace Drivers::PIT

__attribute__((interrupt)) void
IRQ0_timer_handler(IN Interrupts::IDT::InterruptFrame *frame);
