#pragma once

#include "../utils/types.h"

// https://wiki.osdev.org/Serial_Ports#Wires,_Pins,_Connectors_and_the_like
// This address can also be found in the BDA (BIOS Data Area) but its not best practice
#define COM1 0x3F8
#define COM2 0x2F8

//https://wiki.osdev.org/Inline_Assembly/Examples#OUTx
static inline void outb(uint16 port, uint8 val)  {
    __asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

//https://wiki.osdev.org/Inline_Assembly/Examples#INx
static inline uint8 inb(uint16 port) {
    uint8 ret;
    __asm__ volatile ("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}