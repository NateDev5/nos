print_str:
    mov ah, 0x0E

.repeat
    lodsb ; load one byte (char) from SI
    cmp al, 0 ; this check if al is 0
    je .done ; if AL is 0 meaning the string is finished then we jump to the end
    int 0x10 ; call interupt 10 to display the char
    jmp .repeat ; continue displaying the characters

.done:
    ret
