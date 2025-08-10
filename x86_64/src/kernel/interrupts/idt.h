// https://github.com/dreamportdev/Osdev-Notes/blob/master/02_Architecture/05_InterruptHandling.md
// https://pdos.csail.mit.edu/6.828/2005/lec/lec8-slides.pdf

// Old info (For 32 bits)
// For 64 bit only the idt entry structure is different and the IDTR
/*
 * IDT (Interrupt Descriptor Table)
 * Crucial part for handling interrupt in protected mode
 *
 * Its a table that holds interrupts vectors and their corresponding handlers
 *
 * the 'lidt' instruction is used to load it
 * the 'sti' instruction is used to re-enable interrupts
 *
 * The IDT consist of 3 parts:
 *  - IDTR (Interrupt Descriptor Table Register) :
 *      This struct contains the size of the IDT and its address in memory
 *      This struct is what will be used with the 'lidt' instruction to load it
 *
 *      The struct needs to be packed to allow no space between the data
 *      (needs to be that way to achieve the 6 byte (48 bits) size)
 *
 *  - IDT Entry (An entry of the IDT)
 *      Contains:
 *          the low and high address of the Interrupt service routine (The handler for the interrupt)
 *          the selector (check intel manual for more info)
 *          an empty byt
 *          the attributes (gate type, cpu privilege, present bit)
 *      The struct needs to be packed so there wont be any space between the data
 *      (needs to be that way to achieve the 8 byte (64 bits) size)
 *
 *  - IDT (The IDT itself):
 *      Consist of 256 entries (uint8 max) each containing an IDT Entry struct
 *
 *      Entry 0-31 are reserved by the system (they contain exceptions and other interrupts)
 *      a stub for each entry from 0-31 is mandatory to catch exceptions and allow the system to work correctly
 *
 *      The rest needs to be defined by the kernel
 *
 *  ----
 *
 *  When an interrupt is called it needs to be handled by a function that is returned with the 'iret' instruction
 *  either by writing a stub for it in assembly or using the __attribute__((interrupt)) on a function
 *      - Writing it in assembly gives more control over what data is passed to the function and which registers are saved
 *
 *  This received functions receives registers as parameters
 *  An 'End of interrupt' needs to be trigged at the end of the ISR by the 'PIC' to allow other interrupts to be triggered and handled
 */
 
#pragma once

#include <utils/types.h>

#define INTERRUPT_GATE 0x8E // p=1, dpl=0, type=0xE
#define TRAP_GATE      0x8F // p=1, dpl=0, type=0xF

namespace Interrupts::IDT {
// packed attribute used to ensure there are no extra bits

// IDT entry
struct IDT_ENTRY {
    uint16_t isr_add_low;
    uint16_t segment_selector;
    uint8_t  ist;
    uint8_t  type_attributes; // gate type, dpl, p
    uint16_t isr_add_med;
    uint32_t isr_add_high;
    uint32_t zero;
} __attribute__((packed));

// IDT register
struct IDTR {
    uint16_t limit; // the size of the idt need to be one less than the actual size
    uint64_t base;  // base address of IDT
} __attribute__((packed));

void setup();
void set_IDT_entry(IN uint8_t vector, IN PTR handler, IN uint8_t attributes);

struct InterruptFrame {
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t sp;
    uint32_t ss;
};
} // namespace Interrupts::IDT

extern "C" void handle_exception ();
//extern "C" void handle_exception(IN uint32_t vector, IN uint32_t error_code);
