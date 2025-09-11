global IRQ_stub_0

extern handle_irq

%macro IRQ_stub 1
ALIGN 16
IRQ_stub_%+%1:
    mov rdi, %1
    call handle_irq
    iretq
%endmacro

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
