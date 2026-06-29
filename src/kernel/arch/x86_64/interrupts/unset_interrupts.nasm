%macro unset_stub 1
ALIGN 16
unset_stub_%+%1:
    mov rdi, %1
    call handle_unset
    iretq
%endmacro

global unset_stub_0

extern handle_unset

%assign i 0
%rep 255
unset_stub i
%assign i i+1
%endrep
