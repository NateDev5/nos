section .text
global load_gdt

load_gdt:
    lgdt [rdi] ; load the gdt from rdi (the first argument passed (sys v abi))

    push qword 0x08 ; push code segment to stack
    lea rax, [rel .reload_code_segments]
    push qword rax
    retfq ; perform far return

.reload_code_segments:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    retq

