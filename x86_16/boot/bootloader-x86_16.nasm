BITS 16 ; tell nasm to use 16 bits

start:
    mov ax, 07C0h ; put this value into ax aka the starting address
    add ax, 288 ; dont really know why
    mov ss, ax ; copy content ax into the stack segment register
    mov sp, 4096 ; set stack pointer to 4096

    mov ax, 07C0h ; set the current data segment
    mov ds, ax

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


times 510-($-$$) db 0 ; 510 - current address - address of begining of section and put a 0
dw 0xAA55 ; boot signature so it knowns that it is a boot loader