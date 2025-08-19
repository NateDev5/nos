#include <kernel/arch/x86_64/interrupts/idt.h>
#include <kernel/arch/x86_64/interrupts/pic.h>

#include <kernel/library/assert.h>
#include <kernel/library/debug.h>

#include <utils/asm.h>

extern PTR unset_stub_0[];
extern PTR exception_stub_0[];
extern PTR IRQ_stub_0[];

namespace Arch::x86_64::IDT {
IDT_ENTRY idt[256];
PTR       irq_handlers[16];

void setup() {
    PIC::remap();

    IDTR idtr;
    idtr.limit = sizeof(IDT_ENTRY) * 256 - 1;
    idtr.base  = (uint64_t)&idt[0];

    for(uint8_t vector = 0; vector < 255; vector++)
        set_entry(vector, (uint64_t)unset_stub_0 + vector * 16, INTERRUPT_GATE);

    for (uint8_t vector = 0; vector < 32; vector++)
        set_entry(vector, (uint64_t)exception_stub_0 + vector * 16, INTERRUPT_GATE);

    for (uint8_t vector = 0; vector < 16; vector++)
        set_entry(vector + 32, (uint64_t)IRQ_stub_0 + vector * 16, INTERRUPT_GATE);

    __asm__ volatile("lidt %0" ::"m"(idtr));
    sti();

    DEBUG_PRINT("(OK) IDT Initialized")
}

void set_entry(IN uint8_t vector, IN uint64_t handler_addr, IN uint8_t attributes) {
    IDT_ENTRY *descriptor        = &idt[vector];
    descriptor->isr_add_low      = handler_addr & 0xFFFF;
    descriptor->segment_selector = 0x08;
    descriptor->ist              = 0; // not using the Interrupt Stack Table (used for threads)
    descriptor->attributes       = attributes;
    descriptor->isr_add_med      = ((handler_addr >> 16) & 0xFFFF);     // bits 16-32
    descriptor->isr_add_high     = ((handler_addr >> 32) & 0xFFFFFFFF); // last 32 bits
    descriptor->zero             = 0;
}

void set_irq_handler(IN uint8_t irq, IN PTR handler) {
    ASSERT_FATAL(irq < 16, "irq parameter needs to be between 0-15")
    irq_handlers[irq] = handler;
}
} // namespace Arch::x86_64::IDT

void handle_irq(IN uint64_t irq) {
    PTR irq_handler = Arch::x86_64::IDT::irq_handlers[irq];

    if (irq_handler == NULLPTR) {
        DEBUG_PRINT("IRQ%i triggered but has no handler", irq)
        Arch::x86_64::PIC::send_EOI(irq);
        return;
    }

    ((void (*)())irq_handler)(); // call the handler

    Arch::x86_64::PIC::send_EOI(irq);
}

void handle_unset (IN uint64_t vector) {
    DEBUG_PRINT("Interrupt triggered without propper handler (Vector=%i)", vector)
}
