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

