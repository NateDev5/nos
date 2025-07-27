/*
 *
 * PIC (Programmable Interrupt Controller)
 * This controller is what manages harcware interrupts and sends them to the system
 * It can accept multiple interrupts at a time
 *
 * This is CRUCIAL for this controller to be initialized before the IDT is loaded because without this interrupts wont work properly
 * This device is also what need to be sent an 'End of Interrupt' to continue processing them
 *
 * The PIC has 2 chips (a master and a slave)
 * This allows for having more inputs for interrupts
 *
 * The PIC has a design flaw that requires the base vector offset to be set to the start
 * of the external interrupts in the IDT.
 *   (This is crucial because if not the PIC will override the reserved 0-31 interrupts and
 *   the system wont work correctly)
 *
 * If the irq is bigger than 8 then the slave also need an 'End of Interrupt' signal to be sent
 */

#pragma once

// https://pdos.csail.mit.edu/6.828/2014/readings/hardware/8259A.pdf
// https://wiki.osdev.org/8259_PIC
// https://www.geeksforgeeks.org/computer-organization-architecture/operating-modes-of-8259-pic/

#include <utils/types.h>

#define PIC1                      0x20
#define PIC2                      0xA0
#define PIC1_COMMAND              PIC1
#define PIC1_DATA                 PIC1 + 1
#define PIC2_COMMAND              PIC2
#define PIC2_DATA                 PIC2 + 1

#define EXTERNAL_INTERRUPTS_START 0x20

// ICW1 is init
// ICW2 is the offset of the vector
// ICW3 is to tell how the master and slave are wired
// ICW4 is for providing additional info

#define ICW1                      0x10 // init command
#define ICW4                      0x01
#define ICW4_8086                 0x01 // 8086 mode

// end of interrupt
#define PIC_EOI                   0x20

namespace Interrupts::PIC {
void init();
void send_EOI(IN uint8_t irq);
} // namespace Interrupts::PIC
