// https://github.com/dreamportdev/Osdev-Notes/blob/master/02_Architecture/05_InterruptHandling.md
// https://pdos.csail.mit.edu/6.828/2005/lec/lec8-slides.pdf

#pragma once

#include <utils/types.h>

#define INTERRUPT_GATE 0x8E // p=1, dpl=0, type=0xE
#define TRAP_GATE      0x8F // p=1, dpl=0, type=0xF
#define TASK_GATE      0x85 // p=1, dpl=0, type=0x5

namespace Interrupts::IDT {
// packed attribute used to ensure there are no extra bits

// IDT entry
struct IDT_ENTRY {
    uint16_t isr_add_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  attributes; // gate type, dpl (cpu privilege), P (present bit, must
                         // be 1 for the descriptor to be valid)
    uint16_t isr_add_high;
} __attribute__((packed));

// IDT register
struct IDTR {
    uint16_t
        limit; // the size of the idt need to be one less than the actual size
    uint32_t base; // base address of IDT
} __attribute__((packed));

void setup();
void set_IDT_entry(IN uint8_t vector, IN PTR handler, IN uint8_t attributes);

struct InterruptFrame {
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t sp;
    uint32_t ss;
};
} // namespace Interrupts::IDT

extern "C" void handle_exception(IN uint32_t vector, IN uint32_t error_code);
