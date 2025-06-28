ORG 0x8000
BITS 16

start:
    mov si, string
    call print_str

    jmp $

print_str:
    mov ah, 0x0E

.repeat
    lodsb
    cmp al, 0
    je .done
    int 10h
    jmp .repeat

.done:
    ret

string db 'Hello from NOS!', 13, 10, 0