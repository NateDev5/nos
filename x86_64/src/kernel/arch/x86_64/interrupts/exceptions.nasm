%include "kernel/arch/x86_64/interrupts/interrupt_macros.nasm"

%macro exception_stub_with_code 1
ALIGN 16
exception_stub_%+%1:
    push %1 ; push vector
    jmp exception_stub_common
%endmacro

%macro exception_stub 1
ALIGN 16
exception_stub_%+%1:
    push 0 ; push zero because no error code
    push %1 ; push vector
    jmp exception_stub_common
%endmacro

extern handle_exception

exception_stub_common:
    enter_isr
    call handle_exception
    leave_isr

global exception_stub_0

exception_stub 0
exception_stub 1
exception_stub 2
exception_stub 3
exception_stub 4
exception_stub 5
exception_stub 6
exception_stub 7
exception_stub_with_code 8
exception_stub 9
exception_stub_with_code 10
exception_stub_with_code 11
exception_stub_with_code 12
exception_stub_with_code 13
exception_stub_with_code 14
exception_stub 15
exception_stub 16
exception_stub_with_code 17
exception_stub 18
exception_stub 19
exception_stub 20
exception_stub_with_code 21
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
