#include "keyboard.h"
#include "../video/vga.h"
#include "../utils/math.h"
#include "../utils/asm.h"
#include "../interrupts/pic.h"
#include "devices/ps2.h"

KeypressInfo inputBuffer[INPUT_BUFFER_SIZE];
uint8 bufferPos = 0;
uint8 bufferTail = 0;

uint8 keyFlags = 0x0;

bool nextInterruptBreakCode = false;

void initKeyboard(IN bool verbose)
{
    initPS2();
    if (verbose)
        println("(OK) Keyboard initialized", LGREEN);
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
    uint8 scancode = poll();

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

void IRQ1_keyboardHandler(IN InterruptFrame *frame)
{
    processScancode();
    clearScreen();
    for(uint8 i = 0; i < 20; i++) {
        printuint32(inputBuffer[i].scancode, 16, BWHITE);
        printchar('_', BWHITE);
        printuint32(inputBuffer[i].flags, 2, BWHITE);
        printchar('|', BWHITE);
    }
    sendEOI(1);
}