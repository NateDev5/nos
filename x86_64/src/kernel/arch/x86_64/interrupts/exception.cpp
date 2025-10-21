#include <kernel/arch/x86_64/interrupts/exception.h>

#include <kernel/library/log.h>

#include <utils/asm.h>

namespace Arch::x86_64::IDT::Exceptions {
CONST_CHAR_PTR exception_names[22] = {
    "Divide error",
    "Debug exception",
    "NMI (Non maskable interrupt)",
    "Breakpoint",
    "Overflow",
    "BOUND range exceeded",
    "Invalid opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Srack-segment fault",
    "General protection fault",
    "Page fault",
    "",
    "x87 FPU Floating point error",
    "Alignment check",
    "Machine check",
    "SIMD Floating-point exception",
    "Virtualization exception",
    "Control protection exception",
};

void handle_pagefault(IN uint64_t error_code) {
    // cr2 register holds the address causing the page fault
    uint64_t cr2_value;
    asm volatile("mov %%cr2, %0" : "=r"(cr2_value));

    PRINT_EXCEPTION("(%s) %s", (error_code & 2) ? "WRITE" : "READ", (error_code & 1) ? "PAGE PROTECTION VIOLATION" : "NON PRESENT PAGE")
    PRINT_EXCEPTION("Usermode?: %s", (error_code & 4) ? "Yes" : "No")
    PRINT_EXCEPTION("Error code (binary): %b", error_code)

    PRINT_EXCEPTION("Malicious address: %H", cr2_value)
}
} // namespace Arch::x86_64::IDT::Exceptions

void handle_exception(IN Arch::x86_64::IDT::Exceptions::InterruptFrame *interrupt_frame) {
    PRINT_EXCEPTION("-------------------------")

    if (interrupt_frame->vector < 22)
        PRINT_EXCEPTION("Exception occured: (%i) %s", interrupt_frame->vector, Arch::x86_64::IDT::Exceptions::exception_names[interrupt_frame->vector])
    else
        PRINT_EXCEPTION("Unknown exception occured: Vector=%i", interrupt_frame->vector)

    if (interrupt_frame->vector != PAGE_FAULT)
        PRINT_EXCEPTION("Error code: %i (%b)\n", interrupt_frame->error_code, interrupt_frame->error_code)
    else
        Arch::x86_64::IDT::Exceptions::handle_pagefault(interrupt_frame->error_code);

    PRINT_EXCEPTION("RAX=%H, RBX=%H, RCX=%H", interrupt_frame->rax, interrupt_frame->rbx, interrupt_frame->rcx)
    PRINT_EXCEPTION("RDX=%H, RDI=%H, RSI=%H", interrupt_frame->rdx, interrupt_frame->rdi, interrupt_frame->rsi)
    PRINT_EXCEPTION("RBP=%H\n", interrupt_frame->rbp)
    PRINT_EXCEPTION("R8 =%H, R9 =%H, R10=%H", interrupt_frame->r8, interrupt_frame->r9, interrupt_frame->r10)
    PRINT_EXCEPTION("R11=%H, R12=%H, R13=%H", interrupt_frame->r11, interrupt_frame->r12, interrupt_frame->r13)
    PRINT_EXCEPTION("R14=%H, R15=%H\n", interrupt_frame->r14, interrupt_frame->r15)
    PRINT_EXCEPTION("RIP=%H, RSP=%H", interrupt_frame->rip, interrupt_frame->rsp)
    PRINT_EXCEPTION("CS =%H, SS =%H, RFLAGS=%H", interrupt_frame->cs, interrupt_frame->ss, interrupt_frame->rflags)
    PRINT_EXCEPTION("-------------------------")

    cli();
    while (true)
        hlt();
}
