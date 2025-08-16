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
void set_entry(IN uint8_t vector, IN PTR handler, IN uint8_t attributes);
void set_irq_handler(IN PTR handler);

struct InterruptFrame {
    uint64_t error_code;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

} // namespace Arch::x86_64::IDT

extern "C" void handle_irq(IN Arch::x86_64::IDT::InterruptFrame *interrupt_frame);

extern "C" void handle_exception(IN Arch::x86_64::IDT::InterruptFrame *interrupt_frame);

