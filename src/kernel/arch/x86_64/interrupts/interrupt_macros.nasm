
%macro save_gp_regs 0
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
%endmacro

%macro pop_gp_regs 0
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
%endmacro

%macro save_gp_regs_irq 0
; general purpose registers except rdi
push rax
push rbx
push rcx
push rdx
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
%endmacro

%macro pop_gp_regs_irq 0
; pop general purpose registers from stack except rdi
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
pop rdx
pop rcx
pop rbx
pop rax
%endmacro

%macro enter_isr 0
; disable interrupts
cli

save_gp_regs

; pass stack ptr as arg
mov rdi, rsp
%endmacro

%macro leave_isr 0
mov rsp, rax

pop_gp_regs

add rsp, 16 ; remove error code and vector each 8 bytes

; re-enable interrupts
sti

; return from interrupt
iretq
%endmacro


