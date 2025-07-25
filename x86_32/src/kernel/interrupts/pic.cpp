#include <kernel/interrupts/pic.h>

#include <kernel/drivers/video/vga.h>

#include <utils/asm.h>

namespace Interrupts::PIC {
    void init () {
        // ICW1
        outb(PIC1_COMMAND, ICW1 | ICW4); // begin init
        outb(PIC2_COMMAND, ICW1 | ICW4);

        // ICW2
        outb(PIC1_DATA, EXTERNAL_INTERRUPTS_START); // move the offset to the start of the external interrupts because if not it will overwrite reserved interrupts
        outb(PIC2_DATA, EXTERNAL_INTERRUPTS_START + 8); // because there are 8 input lines

        // ICW3
        outb(PIC1_DATA, 4); // tell master PIC there is a slave at IRQ2
        outb(PIC2_DATA, 2); // tell the slave its id

        // ICW4
        outb(PIC1_DATA, ICW4_8086); // set to 8086 mode
        outb(PIC2_DATA, ICW4_8086);

        // unmask the PICs so they can be used
        outb(PIC1_DATA, 0);
    	outb(PIC2_DATA, 0);
    }

    void send_EOI (IN uint8 irq) {
        if(irq >= 8)
            outb(PIC2_COMMAND, PIC_EOI);

        outb(PIC1_COMMAND, PIC_EOI);

    }
}
