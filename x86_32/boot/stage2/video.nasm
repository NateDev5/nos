clear_screen:
    ; change video mode to text mode to clear screen
    mov ah, 0x00
    mov al, 0x03
    int 0x10
