ORG 0x7c00 ; sets the origin basicaly same as mov ax, 07C00h mov ds, ax
BITS 16 

start:
    mov si, reading_disk
    call print_str

    call read_sector

read_sector:
    mov ah, 0x02 ; the mode
    mov al, 1 ; number of sectors to read
    mov ch, 0 ; track/cylinder number 
    mov cl, 2 ; start at two because our bootsector is in the first sector and is already read
    mov dh, 0 ; read with first head
    mov dl, 0x80 ; set the disk to readv (0x80 is first disk)

    mov bx, 0x0000  ; no need for extra because the memory where we want to load can be represented in 16 bits
    mov es, bx
    mov bx, 0x8000 ; safe place to put data

    int 0x13

    jc disk_error ; jump if the carry flag is set to 1 meaning it has an error

    mov si, loading_kernel
    call print_str

    ; change video mode
    mov ah, 0x00
    mov al, 0x03
    int 0x10

    call load_kernel


    ; if kernel dosent load then we display an error msg and make an infinite loop
    mov si, failed_load_kernel
    call print_str

hang:
    jmp hang

load_kernel:
    jmp 0x0000:0x8000 ; jmp to kernel address

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

; data is put at the end to not overwrite instructions
; 13 is carriage return and 10 is move down
reading_disk db 'Reading disk', 13, 10, 0
error_reading_disk db 'Error while reading disk', 13, 10, 0
loading_kernel db 'Loading kernel', 13, 10, 0
failed_load_kernel db 'Failed to load kernel', 13, 10, 0

times 510-($-$$) db 0 ; times repeat instruction db a number of times. 510 because the two last bytes are the signature
dw 0xAA55 ; boot signature so it knowns that it is a boot loader