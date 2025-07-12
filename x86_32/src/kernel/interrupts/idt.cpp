#include <kernel/interrupts/idt.h>
#include <kernel/interrupts/pic.h>

#include <kernel/drivers/hardware/pit.h>
#include <kernel/drivers/video/vga.h>
#include <kernel/drivers/io/keyboard.h>

#include <utils/asm.h>
#include <utils/math.h>


extern PTR stubTable[];
extern PTR timerHandler;

namespace Interrupts::IDT {
    void setup(IN bool verbose)
    {
        PIC::init();

        // the address of the first element of the idt
        idtr.base = (uint32)&idt[0];
        idtr.limit = sizeof(IDT_ENTRY) * 256 - 1;

        // set the first 32 vectors because they are reserved
        for (uint8 vector = 0; vector < 32; vector++)
            setIDTEntry(vector, stubTable[vector], INTERRUPT_GATE);

        setIDTEntry(32, reinterpret_cast<PTR>(IRQ0_timerHandler), INTERRUPT_GATE);
        setIDTEntry(33, reinterpret_cast<PTR>(IRQ1_keyboardHandler), INTERRUPT_GATE);

        // load the idt
        __asm__ volatile("lidt %0" : : "m"(idtr));
        sti();

        if(verbose) Drivers::VGA::println("(OK) IDT initialized", LGREEN);
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

}

extern "C" void handleException(IN uint32 vector, IN uint32 errorCode)
{
    switch (vector)
    {
    case 0:
        Drivers::VGA::errorScreen("Divide error");
        break;
    case 2:
        Drivers::VGA::errorScreen("NMI");
        break;
    case 8:
        Drivers::VGA::errorScreen("Double fault");
        Drivers::VGA::print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        Drivers::VGA::printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    case 10:
        Drivers::VGA::errorScreen("Invalid TSS");
        Drivers::VGA::print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        Drivers::VGA::printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    case 11:
        Drivers::VGA::errorScreen("Segment Not Present");
        Drivers::VGA::print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        Drivers::VGA::printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    case 12:
        Drivers::VGA::errorScreen("Stack-segment fault");
        Drivers::VGA::print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        Drivers::VGA::printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    case 13:
        Drivers::VGA::errorScreen("General protection fault");
        Drivers::VGA::print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        Drivers::VGA::printuint32(errorCode, 2, PRNT_FORMAT(LRED, BLACK));
        break;
    case 14:
        Drivers::VGA::errorScreen("Page fault");
        Drivers::VGA::print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        Drivers::VGA::printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    case 17:
        Drivers::VGA::errorScreen("Alignment check");
        Drivers::VGA::print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        Drivers::VGA::printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    case 21:
        Drivers::VGA::errorScreen("Control protection exception");
        Drivers::VGA::print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        Drivers::VGA::printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    default:
        Drivers::VGA::errorScreen("Unknown error");
        Drivers::VGA::print("Code : ", PRNT_FORMAT(LRED, BLACK));
        Drivers::VGA::printuint32(vector, 16, PRNT_FORMAT(LRED, BLACK));
        Drivers::VGA::print("Error code : ", PRNT_FORMAT(LRED, BLACK));
        Drivers::VGA::printuint32(errorCode, 16, PRNT_FORMAT(LRED, BLACK));
        break;
    }
    cli();
    while (true)
        hlt();
}