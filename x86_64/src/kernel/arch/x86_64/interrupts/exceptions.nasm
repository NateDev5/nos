%include "kernel/arch/x86_64/interrupts/interrupt_macros.nasm"

%macro exception_stub 1
exception_stub_%+%1:
    enter_isr
    call handle_exception
    leave_isr
%endmacro

global exception_stub_0

extern handle_exception

exception_stub 0
exception_stub 1
exception_stub 2
exception_stub 3
exception_stub 4
exception_stub 5
exception_stub 6
exception_stub 7
exception_stub 8
exception_stub 9
exception_stub 10
exception_stub 11
exception_stub 12
exception_stub 13
exception_stub 14
exception_stub 15
exception_stub 16
exception_stub 17
exception_stub 18
exception_stub 19
exception_stub 20
exception_stub 21
exception_stub 22
exception_stub 23
exception_stub 24
exception_stub 25
exception_stub 26
exception_stub 27
exception_stub 28
exception_stub 29
exception_stub 30
exception_stub 31
