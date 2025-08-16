%macro enter_isr 0
mov rdi, rsp
%endmacro

%macro leave_isr 0
mov rsp, rax
iretq
%endmacro


