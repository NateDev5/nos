; %+ is to concantenate strings
; %1 is the argument number 1

%macro interrupt_stub 1
interrupt_stub_%+%1:
    push %1
    call handle_exception
    add esp, 4
    iret
%endmacro

extern handle_exception

interrupt_stub 0
interrupt_stub 1
interrupt_stub 2
interrupt_stub 3
interrupt_stub 4
interrupt_stub 5
interrupt_stub 6
interrupt_stub 7
interrupt_stub 8
interrupt_stub 9
interrupt_stub 10
interrupt_stub 11
interrupt_stub 12
interrupt_stub 13
interrupt_stub 14
interrupt_stub 15
interrupt_stub 16
interrupt_stub 17
interrupt_stub 18
interrupt_stub 19
interrupt_stub 20
interrupt_stub 21
interrupt_stub 22
interrupt_stub 23
interrupt_stub 24
interrupt_stub 25
interrupt_stub 26
interrupt_stub 27
interrupt_stub 28
interrupt_stub 29
interrupt_stub 30
interrupt_stub 31

global stub_table
stub_table:
%assign i 0 
%rep    32 
    dd interrupt_stub_%+i
%assign i i+1 
%endrep
