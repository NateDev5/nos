#include "idt.h"
#include "../utils/asm.h"
#include "../utils/math.h"
#include "../video/video.h"
#include "../utils/timer.h"
#include "pic.h"

extern PTR stubTable[];
extern PTR timerHandler;

void setupIDT()
{
    initPIC();
    
    // the address of the first element of the idt
    idtr.base = (uint32)&idt[0];
    idtr.limit = sizeof(IDT_ENTRY) * 256 - 1;
    
    // set the first 32 vectors because they are reserved
    for(uint8 vector = 0; vector < 32; vector++) {
        setIDTEntry(vector, stubTable[vector], INTERRUPT_GATE);
    }

    setIDTEntry(32, timerHandler, INTERRUPT_GATE);

    // load the idt
    __asm__ volatile("lidt %0" : : "m"(idtr));
    sti();
}

void setIDTEntry (IN uint8 vector, IN PTR handler, IN uint8 attributes) 
{
    IDT_ENTRY* descriptor = &idt[vector];
    descriptor->isrAddLow = ((uint32)handler) & 0xFFFF; // take the first 16 bits of the address
    
    // https://wiki.osdev.org/Segment_Selector
    descriptor->selector = 0x08; // code descriptor in GDT is at index 1 and we want Ring 0 access
    
    descriptor->zero = 0;
    descriptor->attributes = attributes;

    descriptor->isrAddHigh = ((uint32)handler >> 16) & 0xFFFF; // take the last 16 bits
}

void handleException (IN InterruptFrame* frame) {
    errorScreen("Error");
    printuint32(frame->eax, 16);
    printuint32(frame->ip, 16);
    printuint32(frame->cs, 16);
    printuint32(frame->flags, 16);
    printuint32(frame->sp, 16);
    printuint32(frame->ss, 16);
    cli();
    while(true)
       hlt();
}
