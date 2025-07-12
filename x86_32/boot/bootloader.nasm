ORG 0x7c00 ; sets the origin basicaly same as mov ax, 07C00h mov ds, ax
BITS 16 

CODESEG equ code_descriptor - GDT_start
DATASEG equ data_descriptor - GDT_start

read_disk:
    mov ah, 0x02 ; the mode
   ;mov al, 10 <- old (5120)
    mov al, 128 ; number of sectors to read
    mov ch, 0 ; track/cylinder number 
    mov cl, 2 ; start at two because our bootsector is in the first sector and is already read
    mov dh, 0 ; read with first head
    mov dl, 0x80 ; set the disk to readv (0x80 is first disk)

    mov bx, 0x0000  ; no need for extra because the memory where we want to load can be represented in 16 bits
    mov es, bx
    mov bx, 0x8000 ; safe place to put data

    int 0x13

    jc disk_error ; jump if the carry flag is set to 1 meaning it has an error

clear_screen:
    ; change video mode to text mode to clear screen
    mov ah, 0x00
    mov al, 0x03
    int 0x10


; enter protected mode
enter_pm:
    cli ; disable interupts

    lgdt [GDT_descriptor] ; load gdt

    ; change cr0 to 1
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODESEG:start_protected_mode ; far jump to new code seg

    jmp $

disk_error:
    mov si, error_reading_disk
    call print_str

    jmp $

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

; must be at the end of real mode code
GDT_start:
    null_descriptor:
        ; first 8 bytes will be 0s
        dd 0x0 ; 00000000 (4 bytes/32 bits)
        dd 0x0 ; 00000000 (4 bytes/32 bits)
    code_descriptor:
        dw 0xffff ; first 16 bits of the limit (2 bytes/16 bits)

        ; first 24 bits of the base
        dw 0x0 ; 16 bits
        db 0x0 ; 8 bits

        db 0b10011010 ; present, privilige, type (1001) and types flags (1010)
        db 0b11001111 ; other flags (1100) + last 4 bits of limit

        db 0x0 ; last 8 bits of the base
    data_descriptor:
        dw 0xffff
        dw 0x0 
        db 0x0
        db 0b10010010 ; present, privilige, type (1001) and types flags (0010)
        db 0b11001111 ; other flags (1100) + last 4 bits of limit
        db 0x0
GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1 ; size of gdt
    dd GDT_start ; start of gdt

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

error_reading_disk db 'Error while reading disk', 13, 10, 0

times 510-($-$$) db 0 ; times repeat instruction db a number of times. 510 because the two last bytes are the signature
dw 0xAA55 ; boot signature so it knowns that it is a boot loader

