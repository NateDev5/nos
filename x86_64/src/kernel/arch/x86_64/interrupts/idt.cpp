#include <kernel/arch/x86_64/interrupts/idt.h>
#include <kernel/arch/x86_64/interrupts/pic.h>

#include <kernel/library/debug.h>

#include <utils/asm.h>

extern PTR exception_stub_0[];
//extern PTR IRQ0_stub[];
extern "C" void IRQ0_stub();

namespace Arch::x86_64::IDT {
IDT_ENTRY idt[256];

void setup() {
    PIC::remap();

    IDTR idtr;
    idtr.limit = sizeof(IDT_ENTRY) * 256 - 1;
    idtr.base  = (uint64_t)&idt[0];

    for (uint8_t vector = 0; vector < 32; vector++)
        set_entry(vector, exception_stub_0 + (vector * 16), INTERRUPT_GATE);

    set_entry(32, (PTR)IRQ0_stub, INTERRUPT_GATE);

    __asm__ volatile("lidt %0" ::"m"(idtr));
    sti();

    DEBUG_PRINT("(OK) IDT Initialized")
}

void set_entry(IN uint8_t vector, IN PTR handler, IN uint8_t attributes) {
    IDT_ENTRY *descriptor        = &idt[vector];
    uint64_t   handler_addr      = (uint64_t)handler;
    descriptor->isr_add_low      = (handler_addr & 0xFFFF); // first 16 bits
    descriptor->segment_selector = 0x08;
    descriptor->ist              = 0; // not using the Interrupt Stack Table (used for threads)
    descriptor->attributes       = attributes;
    descriptor->isr_add_med      = ((handler_addr >> 16) & 0xFFFF);     // bits 16-32
    descriptor->isr_add_high     = ((handler_addr >> 32) & 0xFFFFFFFF); // last 32 bits
    descriptor->zero             = 0;
}

void set_irq_handler(IN PTR handler) {}

} // namespace Arch::x86_64::IDT

void handle_irq(IN Arch::x86_64::IDT::InterruptFrame *interrupt_frame) { DEBUG_PRINT("IRQ") }

void handle_exception(IN Arch::x86_64::IDT::InterruptFrame *interrupt_frame) { 
    DEBUG_PRINT("Exception") 
    cli();

    while(true)
        hlt();
}

