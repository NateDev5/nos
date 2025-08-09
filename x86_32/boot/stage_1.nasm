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

%include "gdt.nasm"

%include "strings.nasm"
