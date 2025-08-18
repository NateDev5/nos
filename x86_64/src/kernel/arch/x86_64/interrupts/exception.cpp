#include <kernel/arch/x86_64/interrupts/exception.h>

#include <kernel/library/debug.h>

#include <utils/asm.h>

#define PRINT_EXCEPTION(format, ...) \
    DEBUG_PRINT(format, ##__VA_ARGS__) 

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
}

void handle_exception(IN Arch::x86_64::IDT::Exceptions::InterruptFrame *interrupt_frame) {
    PRINT_EXCEPTION("-------------------------")

    if (interrupt_frame->vector < 22)
        PRINT_EXCEPTION("Exception occured: (%i) %s", interrupt_frame->vector, Arch::x86_64::IDT::Exceptions::exception_names[interrupt_frame->vector])
    else
        PRINT_EXCEPTION("Unknown exception occured: Vector=%i", interrupt_frame->vector)

    PRINT_EXCEPTION("Error code: %i\n", interrupt_frame->error_code)
    PRINT_EXCEPTION("RAX=%H, RBX=%H, RCX=%H", interrupt_frame->rax, interrupt_frame->rbx, interrupt_frame->rcx)
    PRINT_EXCEPTION("RDX=%H, RDI=%H, RSI=%H", interrupt_frame->rdx, interrupt_frame->rdi, interrupt_frame->rsi)
    PRINT_EXCEPTION("RBP=%H\n", interrupt_frame->rbp)
    PRINT_EXCEPTION("R8 =%H, R9 =%H, R10=%H", interrupt_frame->r8, interrupt_frame->r9, interrupt_frame->r10)
    PRINT_EXCEPTION("R11=%H, R12=%H, R13=%H", interrupt_frame->r11, interrupt_frame->r12, interrupt_frame->r13)
    PRINT_EXCEPTION("R14=%H, R15=%H\n", interrupt_frame->r14, interrupt_frame->r15)
    PRINT_EXCEPTION("RIP=%H, RSP=%H", interrupt_frame->rip, interrupt_frame->rsp)
    PRINT_EXCEPTION("CS =%H, SS =%H, RFLAGS=%H", interrupt_frame->cs, interrupt_frame->ss, interrupt_frame->rflags)
    PRINT_EXCEPTION("-------------------------")

    while (true)
        hlt();
}
