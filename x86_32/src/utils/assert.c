#include "assert.h"
#include "../video/vga.h"
#include "asm.h"

void assert(IN bool expression, IN const cstr msg)
{
    if (!expression)
    {
        errorScreen(msg);
        cli();
        while (true)
            hlt();
    }
}