ORG 0x7c00 ; sets the origin of the bootsector in memory -> basicaly same as mov ax, 07C00h mov ds, ax
BITS 16

read_disk:
    mov ah, 0x02 ; the mode
    mov al, 10 ; number of sectors to read
    mov ch, 0 ; track/cylinder number 
    mov cl, 2 ; start at two because our bootsector is in the first sector and is already read
    mov dh, 0 ; read with first head
    mov dl, 0x80 ; set the disk to readv (0x80 is first disk)

    mov bx, 0x0000  ; no need for extra because the memory where we want to load can be represented in 16 bits
    mov es, bx
    mov bx, 0x8000 ; safe place to put data (base of free memory area in bios + 512 = 0x8000)

    int 0x13

    jc disk_error ; jump if the carry flag is set to 1 meaning it has an error

jump_to_stage2:
    jmp 0x8000
    jc stage2_error

stage2_error:
    mov si, error_loading_stage2
    call print_str

    jmp $

disk_error:
    mov si, error_reading_disk
    call print_str

    jmp $

%include "utils/print_str.nasm"

error_reading_disk db 'Error while reading disk', 13, 10, 0
error_loading_stage2 db 'Failed to load second stage of bootloader', 13, 10, 0

times 510-($-$$) db 0 ; times repeat instruction db a number of times. 510 because the two last bytes are the signature
dw 0xAA55 ; boot signature so it knowns that it is a boot loader
