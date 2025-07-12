#include "keyboard.h"
#include "../video/vga.h"
#include "../utils/math.h"
#include "../utils/asm.h"
#include "../interrupts/pic.h"
#include "devices/ps2.h"

KeypressInfo inputBuffer[256];
uint8 bufferPos = 0;

uint8 keyFlags = 0x0;

bool nextKeyBreak = false;

void initKeyboard(IN bool verbose)
{
    initPS2();
    if (verbose)
        println("(OK) Keyboard initialized", LGREEN);
}

int8 readKey()
{

    return 0;
}

void processScancode () {
    uint8 flags = 0x01 & nextKeyBreak;
    uint8 scancode = poll();

    if(scancode == BREAK_CODE) {
        nextKeyBreak = true;
        return;
    }

    switch (scancode)
    {
    case SCANCODE_ALT:
        //         ↓    
        // 0000 0001
        keyFlags = nextKeyBreak ? (keyFlags | 0x01) : (keyFlags & ~0x01);
        break;
    case SCANCODE_CTRL:
        //        ↓    
        // 0000 0010
        keyFlags = nextKeyBreak ? (keyFlags | 0x02) : (keyFlags & ~0x02);
        break;
    case SCANCODE_SHIFT:
        //       ↓    
        // 0000 0100
        keyFlags = nextKeyBreak ? 0x04 : 0x0;
        break;
    case SCANCODE_CAPS:
        //      ↓    
        // 0000 1000
        keyFlags |= nextKeyBreak ? 0x08 : 0x0;
        break;
    default:
        break;
    }

    printuint32(keyFlags, 2, BWHITE);
    print(" | ", BWHITE);


    inputBuffer[bufferPos].scancode = scancode;
    inputBuffer[bufferPos].flags = flags;

    bufferPos++;
    if (bufferPos > 255)
        bufferPos = 0;

    nextKeyBreak = false;
}

void IRQ1_keyboardHandler(IN InterruptFrame *frame)
{
    processScancode();
    sendEOI(1);
}