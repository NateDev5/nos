#pragma once

#include "../utils/types.h"

// https://wiki.osdev.org/Inline_Assembly/Examples#OUTx
static inline void outb(uint16 port, uint8 val)
{
    __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

// https://wiki.osdev.org/Inline_Assembly/Examples#INx
static inline uint8 inb(uint16 port)
{
    uint8 ret;
    __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

static inline void nop()
{
    __asm__ volatile("nop");
}

static inline void hlt()
{
    __asm__ volatile("hlt");
}

// clear interrupt flag
static inline void cli()
{
    __asm__ volatile("cli");
}

// set interrupt flag
static inline void sti()
{
    __asm__ volatile("sti");
}

static inline void ioWait(void)
{
    outb(0x80, 0);
}