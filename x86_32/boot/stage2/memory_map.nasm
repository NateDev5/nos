test_area: resb 24

detect_memory:
    mov ax, seg test_area
    mov es, ax

    mov eax, 0xE820
    mov ebx, 0


