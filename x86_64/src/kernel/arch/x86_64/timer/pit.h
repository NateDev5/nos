/*
 *
 * PIT (Programmable Interrupt Timer)
 * Generates interrupts at a set interval depending on mode to IRQ0
 *
 * The divisor is set to define the interval at which an interrupt will be triggered
 * ----
 *
 * For each millisecond:
 *  pit_input_clock (1.19MHz = ~1 second) / 1000
 *  1193182 / 1000 = interrupt each millisecond
 * ----
 *
 * Divisor is 16bit (2 bytes) and needs to be sent to the PIT in two parts
 * to the PIT CHANNEL 0 port
 * ----
 *
 * To first set the divisor the PIT needs to be initialized by sending a command
 * to the PIT COMMAND REGISTER port
 *
 * Command is 8 bits (1 byte)
 * bit 7-6: Counter
 * bit 5-4: Read Write Mode (1 = LSB, 2 = MSB, 3 = LSB then MSB)
 * bit 3-1: The mode (
 *                      0 = Interrupt on terminal count
 *                      1 = Hardware Retriggerable one shot
 *                      2 = Rate Generator
 *                      3 = Square Wave Mode
 *                      4 = Software Strobe
 *                      5 = Hardware Strobe
 *                   )
 * bit 0: BCD (0 = 16-bit counter, 1 = 4x BCD decade counters)
 *
 *
 *
 * Read Write Mode is basically the order in which the divisor will be read
 *
 * ----
 */

// http://www.osdever.net/bkerndev/Docs/pit.htm
// https://www.cpcwiki.eu/imgs/e/e3/8253.pdf

#pragma once

#include <utils/types.h>

#define PIT_CHANNEL_0        0x40
#define PIT_COMMAND_REGISTER 0x43

namespace Arch::x86_64::PIT {
///
/// Setup the Programmable Interrupt Timer
/// Sets the mod and the inteval at which an interrupt is triggered
///
void init();
void sleep(IN uint32_t milliseconds);
void timer_handler();
} // namespace Arch::x86_64::PIT

//__attribute__((interrupt)) void IRQ0_timer_handler(IN Interrupts::IDT::InterruptFrame *frame);
