section .data
global count
count dw 0

section .text
global timerHandler

extern sendEOI

timerHandler:
    push eax
    mov  eax, [count]
    test eax, eax
    dec eax
    mov [count], eax
    iret
    call sendEOI