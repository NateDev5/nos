#include <kernel/interrupts/idt.h>
#include <kernel/interrupts/pic.h>

#include <kernel/drivers/hardware/pit.h>
#include <kernel/drivers/video/vga.h>
#include <kernel/drivers/io/keyboard.h>

#include <kernel/library/log.h>
#include <kernel/library/panic.h>

#include <utils/asm.h>
#include <utils/math.h>


extern PTR stubTable[];
extern PTR timerHandler;

namespace Interrupts::IDT {
    static IDT_ENTRY idt[256]; // the actual idt;
    static IDTR idtr;

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

        if(verbose) Library::fprintln("(OK) IDT initialized", Drivers::VGA::Color::LGREEN);
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
        Kernel::errorScreen("Divide error");
        break;
    case 2:
        Kernel::errorScreen("NMI");
        break;
    case 8:
        Kernel::errorScreen("Double fault");
        Library::printf("Error code : %h", errorCode);
        Drivers::VGA::setBackgroundColor(Drivers::VGA::LRED);
        Drivers::VGA::setForegroundColor(Drivers::VGA::BLACK);
        break;
    case 10:
        Kernel::errorScreen("Invalid TSS");
        Library::printf("Error code : %h", errorCode);
        Drivers::VGA::setBackgroundColor(Drivers::VGA::LRED);
        Drivers::VGA::setForegroundColor(Drivers::VGA::BLACK);
        break;
    case 11:
        Kernel::errorScreen("Segment Not Present");
        Library::printf("Error code : %h", errorCode);
        Drivers::VGA::setBackgroundColor(Drivers::VGA::LRED);
        Drivers::VGA::setForegroundColor(Drivers::VGA::BLACK);
        break;
    case 12:
        Kernel::errorScreen("Stack-segment fault");
        Library::printf("Error code : %h", errorCode);
        Drivers::VGA::setBackgroundColor(Drivers::VGA::LRED);
        Drivers::VGA::setForegroundColor(Drivers::VGA::BLACK);
        break;
    case 13:
        Kernel::errorScreen("General protection fault");
        Library::printf("Error code : %h", errorCode);
        Drivers::VGA::setBackgroundColor(Drivers::VGA::LRED);
        Drivers::VGA::setForegroundColor(Drivers::VGA::BLACK);
        break;
    case 14:
        Kernel::errorScreen("Page fault");
        Library::printf("Error code : %h", errorCode);
        Drivers::VGA::setBackgroundColor(Drivers::VGA::LRED);
        Drivers::VGA::setForegroundColor(Drivers::VGA::BLACK);
        break;
    case 17:
        Kernel::errorScreen("Alignment check");
        Library::printf("Error code : %h", errorCode);
        Drivers::VGA::setBackgroundColor(Drivers::VGA::LRED);
        Drivers::VGA::setForegroundColor(Drivers::VGA::BLACK);
        break;
    case 21:
        Kernel::errorScreen("Control protection exception");
        Library::printf("Error code : %h", errorCode);
        Drivers::VGA::setBackgroundColor(Drivers::VGA::LRED);
        Drivers::VGA::setForegroundColor(Drivers::VGA::BLACK);
        break;
    default:
        Kernel::errorScreen("Unknown error");
        Library::printf("Code : %h\nError code : %h", vector, errorCode);
        Drivers::VGA::setBackgroundColor(Drivers::VGA::LRED);
        Drivers::VGA::setForegroundColor(Drivers::VGA::BLACK);
        break;
    }
    cli();
    while (true)
        hlt();
}