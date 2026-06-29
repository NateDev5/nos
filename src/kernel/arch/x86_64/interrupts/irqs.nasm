%include "kernel/arch/x86_64/interrupts/interrupt_macros.nasm"

global IRQ_stub_0

extern handle_irq

%macro IRQ_stub 1
ALIGN 16
IRQ_stub_%+%1:
    push rdi
    mov rdi, %1
    jmp IRQ_common
%endmacro

IRQ_common:
    save_gp_regs_irq
    call handle_irq
    pop_gp_regs_irq
    pop rdi
    iretq

IRQ_stub 0
IRQ_stub 1
IRQ_stub 2
IRQ_stub 3
IRQ_stub 4
IRQ_stub 5
IRQ_stub 6
IRQ_stub 7
IRQ_stub 8
IRQ_stub 9
IRQ_stub 10
IRQ_stub 11
IRQ_stub 12
IRQ_stub 13
IRQ_stub 14
IRQ_stub 15
