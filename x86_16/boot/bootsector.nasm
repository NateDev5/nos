BITS 16 ; tell nasm to use 16 bits

start:
    mov ax, 07C0h 
    mov ds, ax ; set the current data segment

    mov si, string ; put the pointer of the string into register SI so that the print string can know what to print
    call print_str ; call print_str

    jmp $

    string db 'Hello from NOS!', 0 ; assign characters to the string

print_str:
    mov ah, 0Eh

.repeat
    lodsb ; load one byte (char) from SI
    cmp al, 0 ; this check if al is 0
    je .done ; if AL is 0 meaning the string is finished then we jump to the end
    int 10h ; call interupt 10 to display the char
    jmp .repeat ; continue displaying the characters


.done:
    ret


times 510-($-$$) db 0 ; times repeat instruction db a number of times. 510 because the two last bytes are the signature
dw 0xAA55 ; boot signature so it knowns that it is a boot loader