//https://github.com/dreamportdev/Osdev-Notes/blob/master/02_Architecture/05_InterruptHandling.md
//https://pdos.csail.mit.edu/6.828/2005/lec/lec8-slides.pdf

#pragma once

#include "../utils/types.h"

#define INTERRUPT_GATE       0x8E // p=1, dpl=0, type=0xE
#define TRAP_GATE            0x8F // p=1, dpl=0, type=0xF
#define TASK_GATE            0x85 // p=1, dpl=0, type=0x5

// packed attribute used to ensure there are no extra bits

// IDT entry
typedef struct IDT_ENTRY
{
    uint16 isrAddLow;
    uint16 selector;
    uint8 zero;
    uint8 attributes; // gate type, dpl (cpu privilege), P (present bit, must be 1 for the descriptor to be valid)
    uint16 isrAddHigh;
} __attribute__((packed)) IDT_ENTRY;

// IDT register
typedef struct IDTR
{
    uint16 limit; // the size of the idt need to be one less than the actual size
    uint32 base; // base address of IDT
} __attribute__((packed)) IDTR;

static IDT_ENTRY idt[256]; // the actual idt;
static IDTR idtr;

void setupIDT();
void setIDTEntry (IN uint8 vector, IN PTR handler, IN uint8 attributes);

void handleException (IN uint32 vector, IN uint32 errorCode);

typedef struct InterruptFrame {
    uint32 ip;
    uint32 cs;
    uint32 flags;
    uint32 sp;
    uint32 ss;
} InterruptFrame;