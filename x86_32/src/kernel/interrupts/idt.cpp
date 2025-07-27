#include <kernel/interrupts/idt.h>
#include <kernel/interrupts/pic.h>

#include <kernel/drivers/hardware/pit.h>
#include <kernel/drivers/io/keyboard.h>
#include <kernel/drivers/video/vga.h>

#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/panic.h>

#include <utils/asm.h>
#include <utils/math.h>

extern PTR stub_table[];
extern PTR timer_handler;

namespace Interrupts::IDT {
static IDT_ENTRY idt[256]; // the actual idt;
static IDTR      idtr;

void setup() {
    PIC::init();

    // the address of the first element of the idt
    idtr.base  = (uint32_t)&idt[0];
    idtr.limit = sizeof(IDT_ENTRY) * 256 - 1;

    // set the first 32 vectors because they are reserved
    for (uint8_t vector = 0; vector < 32; vector++)
        set_IDT_entry(vector, stub_table[vector], INTERRUPT_GATE);

    set_IDT_entry(32, reinterpret_cast<PTR>(IRQ0_timer_handler), INTERRUPT_GATE);
    set_IDT_entry(33, reinterpret_cast<PTR>(IRQ1_keyboard_handler), INTERRUPT_GATE);

    // load the idt
    __asm__ volatile("lidt %0" : : "m"(idtr));
    sti();

    DEBUG_PRINT("(OK) IDT initialized")
}

void set_IDT_entry(IN uint8_t vector, IN PTR handler, IN uint8_t attributes) {
    IDT_ENTRY *descriptor   = &idt[vector];
    descriptor->isr_add_low = ((uint32_t)handler) & 0xFFFF; // take the first 16 bits of the address

    // https://wiki.osdev.org/Segment_Selector
    descriptor->selector = 0x08; // code descriptor in GDT is at index 1 and we want Ring 0 access

    descriptor->zero       = 0;
    descriptor->attributes = attributes;

    descriptor->isr_add_high = ((uint32_t)handler >> 16) & 0xFFFF; // take the last 16 bits
}

} // namespace Interrupts::IDT

extern "C" void handle_exception(IN uint32_t vector, IN uint32_t error_code) {
    switch (vector) {
    case 0:
        Kernel::error_screen("Divide error");
        break;
    case 2:
        Kernel::error_screen("NMI");
        break;
    case 8:
        Kernel::error_screen("Double fault");
        Library::printf("Error code : %h", error_code);
        Drivers::VGA::set_screen_back_color(Drivers::VGA::LRED);
        Drivers::VGA::set_screen_fore_color(Drivers::VGA::BLACK);
        break;
    case 10:
        Kernel::error_screen("Invalid TSS");
        Library::printf("Error code : %h", error_code);
        Drivers::VGA::set_screen_back_color(Drivers::VGA::LRED);
        Drivers::VGA::set_screen_fore_color(Drivers::VGA::BLACK);
        break;
    case 11:
        Kernel::error_screen("Segment Not Present");
        Library::printf("Error code : %h", error_code);
        Drivers::VGA::set_screen_back_color(Drivers::VGA::LRED);
        Drivers::VGA::set_screen_fore_color(Drivers::VGA::BLACK);
        break;
    case 12:
        Kernel::error_screen("Stack-segment fault");
        Library::printf("Error code : %h", error_code);
        Drivers::VGA::set_screen_back_color(Drivers::VGA::LRED);
        Drivers::VGA::set_screen_fore_color(Drivers::VGA::BLACK);
        break;
    case 13:
        Kernel::error_screen("General protection fault");
        Library::printf("Error code : %h", error_code);
        Drivers::VGA::set_screen_back_color(Drivers::VGA::LRED);
        Drivers::VGA::set_screen_fore_color(Drivers::VGA::BLACK);
        break;
    case 14:
        Kernel::error_screen("Page fault");
        Library::printf("Error code : %h", error_code);
        Drivers::VGA::set_screen_back_color(Drivers::VGA::LRED);
        Drivers::VGA::set_screen_fore_color(Drivers::VGA::BLACK);
        break;
    case 17:
        Kernel::error_screen("Alignment check");
        Library::printf("Error code : %h", error_code);
        Drivers::VGA::set_screen_back_color(Drivers::VGA::LRED);
        Drivers::VGA::set_screen_fore_color(Drivers::VGA::BLACK);
        break;
    case 21:
        Kernel::error_screen("Control protection exception");
        Library::printf("Error code : %h", error_code);
        Drivers::VGA::set_screen_back_color(Drivers::VGA::LRED);
        Drivers::VGA::set_screen_fore_color(Drivers::VGA::BLACK);
        break;
    default:
        Kernel::error_screen("Unknown error");
        Library::printf("Code : %h\nError code : %h", vector, error_code);
        Drivers::VGA::set_screen_back_color(Drivers::VGA::LRED);
        Drivers::VGA::set_screen_fore_color(Drivers::VGA::BLACK);
        break;
    }
    cli();
    while (true)
        hlt();
}
