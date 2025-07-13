#include <kernel/drivers/io/keyboard.h>
#include <kernel/drivers/video/vga.h>
#include <kernel/drivers/io/keymap.h>

#include <kernel/interrupts/idt.h>
#include <kernel/interrupts/pic.h>

#include <kernel/library/log.h>

#include <kernel/devices/io/ps2.h>

#include <utils/math.h>
#include <utils/asm.h>

namespace Drivers::Keyboard {
    KeypressInfo inputBuffer[INPUT_BUFFER_SIZE];
    uint8 bufferPos = 0;
    uint8 bufferTail = 0;

    uint8 keyFlags = 0x0;

    bool nextInterruptBreakCode = false;
    bool nextInterruptExtendedCode = false;

    void init(IN bool verbose)
    {
        Devices::PS2::init();
        if (verbose)
            Library::fprintln("(OK) Keyboard initialized", Drivers::VGA::LGREEN);
    }

    KeypressInfo readKey()
    {
        if(inputBuffer[bufferTail].scancode == SCANCODE_INVALID) return {};

        KeypressInfo info = inputBuffer[bufferTail];
        inputBuffer[bufferTail].scancode = SCANCODE_INVALID;
        inputBuffer[bufferTail].keycode = KEYCODE_INVALID;
        inputBuffer[bufferTail].flags = 0;

        bufferTail = (bufferTail + 1) % INPUT_BUFFER_SIZE;
        return { info };
    }

    void readLine(OUT KeypressInfo *buffer)
    {

    }

    uint8 getKeycode (IN uint8 scancode, IN bool extended) {
        auto map = extended ? keycodeMapExtended : keycodeMap;
        auto len = extended ? KEYCODE_MAP_EXTENDED_LEN : KEYCODE_MAP_LEN;

        for(uint8 i = 0; i < len; i++)
            if(map[i][0] == scancode)
                return map[i][1];

        return KEYCODE_INVALID;
    }

    void processScancode()
    {
        uint8 flags = 0x01 & !nextInterruptBreakCode;
        uint8 scancode = Devices::PS2::poll();

        if(scancode == EXTENDED_CODE) 
        {
            nextInterruptExtendedCode = true;
            return;
        }

        if (scancode == BREAK_CODE) 
        {
            nextInterruptBreakCode = true;
            return;
        }

        if(!nextInterruptExtendedCode)
            switch (scancode)
            {
            case SCANCODE_ALT:
                keyFlags = nextInterruptBreakCode ? (keyFlags & ~ALT) : (keyFlags | ALT);
                break;
            case SCANCODE_CTRL:
                keyFlags = nextInterruptBreakCode ? (keyFlags & ~CTRL) : (keyFlags | CTRL);
                break;
            case SCANCODE_SHIFT:
                keyFlags = nextInterruptBreakCode ? (keyFlags & ~SHIFT) : (keyFlags | SHIFT);
                break;
            case SCANCODE_CAPS:
                if (nextInterruptBreakCode)
                    break;
                keyFlags ^= CAPS;
                break;
            default:
                break;
            }

        flags |= keyFlags;

        if(nextInterruptExtendedCode)
            flags |= EXTENDED;

        inputBuffer[bufferPos].scancode = scancode;
        inputBuffer[bufferPos].flags = flags;
        inputBuffer[bufferPos].keycode = getKeycode(scancode, nextInterruptExtendedCode);

        bufferPos = (bufferPos + 1) % INPUT_BUFFER_SIZE;

        nextInterruptBreakCode = false;
        nextInterruptExtendedCode = false;
    }
}

void IRQ1_keyboardHandler(IN Interrupts::IDT::InterruptFrame *frame)
{
    Drivers::Keyboard::processScancode();
    Interrupts::PIC::sendEOI(1);
}