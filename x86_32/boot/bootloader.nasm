ORG 0x7c00 ; sets the origin basicaly same as mov ax, 07C00h mov ds, ax

%include "stage_1.nasm"

BITS 32

start_protected_mode:
    ; set all register to start of data segment
    mov eax, DATASEG
	mov ds, eax
	mov ss, eax
	mov es, eax
	mov fs, eax
	mov gs, eax

    mov ebp, 0x90000		; 32 bit stack base pointer
	mov esp, ebp           ; set stack ptr to base ptr

    jmp 0x8000 ; jump to where kernel is located
    jmp $

times 510-($-$$) db 0 ; times repeat instruction db a number of times. 510 because the two last bytes are the signature
dw 0xAA55 ; boot signature so it knowns that it is a boot loader
