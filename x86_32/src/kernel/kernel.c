#include "../memory/mem.h"
#include "../utils/math.h"
#include "../video/vga.h"
#include "../io/keyboard.h"
#include "../interrupts/idt.h"
#include "../utils/asm.h"
#include "../utils/pit.h"
#include "regression_test_kernel.h"

void kmain()
{
    testKernel();

    initKeyboard(true); // before setting up interrupts

    initPIT(true);
    setupIDT(true);
    disableCursor();

    /*
    KeypressInfo lineBuffer[256];
    readLine(lineBuffer);
    for(uint8 i = 0; i < 256; i++)
        printuint32(lineBuffer[i].scancode, 16, BWHITE);
    */
   
    while (true)
    {
    }
}