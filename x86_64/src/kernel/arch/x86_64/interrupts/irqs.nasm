section .text
%include "kernel/arch/x86_64/interrupts/interrupt_macros.nasm"

global IRQ0_stub

extern handle_irq

IRQ0_stub:
    enter_isr
    call handle_irq
    leave_isr
