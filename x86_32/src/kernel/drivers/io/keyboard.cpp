#include <kernel/drivers/io/keyboard.h>
#include <kernel/drivers/video/vga.h>

#include <kernel/interrupts/idt.h>
#include <kernel/interrupts/pic.h>

#include <kernel/devices/ps2.h>

#include <utils/math.h>
#include <utils/asm.h>

namespace Drivers::Keyboard {
    KeypressInfo inputBuffer[INPUT_BUFFER_SIZE];
    uint8 bufferPos = 0;
    uint8 bufferTail = 0;

    uint8 keyFlags = 0x0;

    bool nextInterruptBreakCode = false;

    void init(IN bool verbose)
    {
        Devices::PS2::init();
        if (verbose)
            VGA::println("(OK) Keyboard initialized", LGREEN);
    }

    void readLine(OUT KeypressInfo *buffer)
    {

    }

    int8 readKey()
    {

        return 0;
    }

    void processScancode()
    {
        uint8 flags = 0x01 & !nextInterruptBreakCode;
        uint8 scancode = Devices::PS2::poll();

        if (scancode == BREAK_CODE)
        {
            nextInterruptBreakCode = true;
            return;
        }

        switch (scancode)
        {
        case SCANCODE_ALT:
            keyFlags = nextInterruptBreakCode ? (keyFlags & ~ALT_FLAG) : (keyFlags | ALT_FLAG);
            break;
        case SCANCODE_CTRL:
            keyFlags = nextInterruptBreakCode ? (keyFlags & ~CTRL_FLAG) : (keyFlags | CTRL_FLAG);
            break;
        case SCANCODE_SHIFT:
            keyFlags = nextInterruptBreakCode ? (keyFlags & ~SHIFT_FLAG) : (keyFlags | SHIFT_FLAG);
            break;
        case SCANCODE_CAPS:
            if (nextInterruptBreakCode)
                break;
            keyFlags ^= CAPS_FLAG;
            break;
        default:
            break;
        }

        flags |= (keyFlags << 1);

        inputBuffer[bufferPos].scancode = scancode;
        inputBuffer[bufferPos].flags = flags;

        bufferPos++;
        if (bufferPos > 255)
            bufferPos = 0;

        nextInterruptBreakCode = false;
    }
}

void IRQ1_keyboardHandler(IN Interrupts::IDT::InterruptFrame *frame)
{
    Drivers::Keyboard::processScancode();
    Interrupts::PIC::sendEOI(1);
}