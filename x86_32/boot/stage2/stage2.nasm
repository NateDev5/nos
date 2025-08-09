BITS 16

CODESEG equ code_descriptor - GDT_start
DATASEG equ data_descriptor - GDT_start

loaded:
    mov si, log_loaded_second_stage
    call print_str

read_disk:
    mov ah, 0x02 ; the mode
    mov al, 117 ; number of sectors to read
    mov ch, 0 ; track/cylinder number 
    mov cl, 12 ; start at 11 because our second stage is in sectors 2-12 and is already read
    mov dh, 0 ; read with first head
    mov dl, 0x80 ; set the disk to readv (0x80 is first disk)

    mov bx, 0x0000  ; no need for extra because the memory where we want to load can be represented in 16 bits
    mov es, bx
    mov bx, 0x9400 ; 0x8000 + 5120 = 0x9400

    int 0x13

    jc disk_error ; jump if the carry flag is set to 1 meaning it has an error

%include "stage2/video.nasm"

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

%include "utils/print_str.nasm"
%include "stage2/gdt.nasm"

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

    jmp 0x9400 ; jump to where kernel is located
    jmp $

log_loaded_second_stage db 'Successfully loaded second stage', 13, 10, 0

error_failed_to_load_kernel db 'Failed to load kernel', 13, 10, 0
error_reading_disk db 'Error while reading disk', 13, 10, 0
