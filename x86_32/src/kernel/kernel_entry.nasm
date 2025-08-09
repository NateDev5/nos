BITS 32

extern kmain

kernel_entry:
    push eax ; push the pointer of the bootloader info
    call kmain
