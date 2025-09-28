#pragma once

#include <utils/types.h>

#define PAGE_FAULT 14

namespace Arch::x86_64::IDT::Exceptions {

struct InterruptFrame {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;

    uint64_t rbp;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t vector;
    uint64_t error_code;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

void handle_pagefault (IN uint64_t error_code); 
} // namespace Arch::x86_64::IDT::Exceptions

extern "C" void handle_exception(IN Arch::x86_64::IDT::Exceptions::InterruptFrame *interrupt_frame);
