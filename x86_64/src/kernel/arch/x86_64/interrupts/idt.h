// https://github.com/dreamportdev/Osdev-Notes/blob/master/02_Architecture/05_InterruptHandling.md
// https://pdos.csail.mit.edu/6.828/2005/lec/lec8-slides.pdf

#pragma once

#include <utils/types.h>

#define INTERRUPT_GATE 0x8E // p=1, dpl=0, type=0xE
#define TRAP_GATE      0x8F // p=1, dpl=0, type=0xF

namespace Arch::x86_64::IDT {
struct IDT_ENTRY {
    uint16_t isr_add_low;      // First 16 bits of ISR
    uint16_t segment_selector; // Code segment selector from GDT
    uint8_t  ist;              // 0 or (Bits 0..2) needs to hold the offset of the Interrupt stack table
    uint8_t  attributes;       // gate type, dpl, p
    uint16_t isr_add_med;      // Bits 16-32 of the ISR address
    uint32_t isr_add_high;     // Last 32 bits of the ISR address
    uint32_t zero;             // Reserved needs to be 0
} __attribute__((packed));     // Packed used to remove ant extra padding between values to make it 16 bytes

struct IDTR {
    uint16_t limit;        // the size of the idt need to be one less than the actual size
    uint64_t base;         // base address of IDT
} __attribute__((packed)); // packed is needed

void setup();
void set_entry(IN uint8_t vector, IN uint64_t handler_addr, IN uint8_t attributes);
void set_irq_handler(IN uint8_t irq, IN PTR handler);

} // namespace Arch::x86_64::IDT

extern "C" void handle_irq(IN uint64_t irq);
extern "C" void handle_unset (IN uint64_t vector);
