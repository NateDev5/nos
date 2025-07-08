#include "idt.h"
#include "../utils/asm.h"
#include "../utils/math.h"
#include "../video/vga.h"
#include "../utils/pit.h"
#include "../io/keyboard.h"
#include "pic.h"

extern PTR stubTable[];
extern PTR timerHandler;

void setupIDT(IN bool verbose)
{
    initPIC();

    // the address of the first element of the idt
    idtr.base = (uint32)&idt[0];
    idtr.limit = sizeof(IDT_ENTRY) * 256 - 1;

    // set the first 32 vectors because they are reserved
    for (uint8 vector = 0; vector < 32; vector++)
        setIDTEntry(vector, stubTable[vector], INTERRUPT_GATE);

    setIDTEntry(32, IRQ0_timerHandler, INTERRUPT_GATE);
    setIDTEntry(33, IRQ1_keyboardHandler, INTERRUPT_GATE);

    // load the idt
    __asm__ volatile("lidt %0" : : "m"(idtr));
    sti();

    if(verbose) println("(OK) IDT initialized", LGREEN);
}

void setIDTEntry(IN uint8 vector, IN PTR handler, IN uint8 attributes)
{
    IDT_ENTRY *descriptor = &idt[vector];
    descriptor->isrAddLow = ((uint32)handler) & 0xFFFF; // take the first 16 bits of the address

    // https://wiki.osdev.org/Segment_Selector
    descriptor->selector = 0x08; // code descriptor in GDT is at index 1 and we want Ring 0 access

    descriptor->zero = 0;
    descriptor->attributes = attributes;

    descriptor->isrAddHigh = ((uint32)handler >> 16) & 0xFFFF; // take the last 16 bits
}

void handleException(IN uint32 vector, IN uint32 errorCode)
{
    switch (vector)
    {
    case 0:
        errorScreen("Divide error");
        break;
    case 2:
        errorScreen("NMI");
        break;
    case 8:
        errorScreen("Double fault");
        print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    case 10:
        errorScreen("Invalid TSS");
        print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    case 11:
        errorScreen("Segment Not Present");
        print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    case 12:
        errorScreen("Stack-segment fault");
        print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    case 13:
        errorScreen("General protection fault");
        print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        printuint32(errorCode, 2, PRNT_FORMAT(LRED, BLACK));
        break;
    case 14:
        errorScreen("Page fault");
        print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    case 17:
        errorScreen("Alignment check");
        print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    case 21:
        errorScreen("Control protection exception");
        print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    default:
        errorScreen("Unknown error");
        print("Code : ", PRNT_FORMAT(LRED, BLACK));
        printuint32(vector, 16, PRNT_FORMAT(LRED, BLACK));
        print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    }
    cli();
    while (true)
        hlt();
}