; %+ is to concantenate strings
; %1 is the argument number 1

%macro interruptStub 1
interruptStub_%+%1:
    push %1
    call handleException
    add esp, 4
    iret
%endmacro

extern handleException

interruptStub 0
interruptStub 1
interruptStub 2
interruptStub 3
interruptStub 4
interruptStub 5
interruptStub 6
interruptStub 7
interruptStub 8
interruptStub 9
interruptStub 10
interruptStub 11
interruptStub 12
interruptStub 13
interruptStub 14
interruptStub 15
interruptStub 16
interruptStub 17
interruptStub 18
interruptStub 19
interruptStub 20
interruptStub 21
interruptStub 22
interruptStub 23
interruptStub 24
interruptStub 25
interruptStub 26
interruptStub 27
interruptStub 28
interruptStub 29
interruptStub 30
interruptStub 31

global stubTable
stubTable:
%assign i 0 
%rep    32 
    dd interruptStub_%+i
%assign i i+1 
%endrep