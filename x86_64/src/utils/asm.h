#pragma once

#include <kernel/library/debug.h>
#include <utils/types.h>

// https://wiki.osdev.org/Inline_Assembly/Examples#OUTx
static inline void outb(IN uint16_t port, IN uint8_t val) { __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory"); }

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

static inline void nop() { __asm__ volatile("nop"); }

static inline void hlt() { __asm__ volatile("hlt"); }

// clear interrupt flag
static inline void cli() { __asm__ volatile("cli"); }

// set interrupt flag
static inline void sti() { __asm__ volatile("sti"); }

static inline void io_wait(void) { outb(0x80, 0); }
