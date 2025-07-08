#include "keyboard.h"
#include "../video/vga.h"
#include "../utils/math.h"
#include "../utils/asm.h"
#include "../interrupts/pic.h"
#include "ps2.h"

void initKeyboard (IN bool verbose) {
    initPS2();
    if(verbose) println("(OK) Keyboard initialized", LGREEN);
}

int8 readKey()
{ 

    return 0;
}

void IRQ1_keyboardHandler (IN InterruptFrame* frame) {
    // do something
    printuint32(poll(), 16, BWHITE);
    sendEOI(1);
}