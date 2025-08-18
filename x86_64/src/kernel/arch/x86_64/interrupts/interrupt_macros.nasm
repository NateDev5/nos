%macro enter_isr 0
; disable interrupts
cli

; general purpose registers
push rax
push rbx
push rcx
push rdx
push rdi
push rsi
push rbp

push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15

; push stack pointer as arg
mov rdi, rsp
%endmacro

%macro leave_isr 0
mov rsp, rax

; pop general purpose registers from stack
pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8

pop rbp
pop rsi
pop rdi
pop rdx
pop rcx
pop rbx
pop rax

add rsp, 16 ; remove error code and vector each 8 bytes

; re-enable interrupts
sti

; return from interrupt
iretq
%endmacro


