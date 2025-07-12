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
    // if (!initPS2Controller())
    //     println("Failed to init ps2 controller", LRED);

    //int i = 0/0;
    //__asm__ volatile ("int $0x12");

    
    while (true)
    {
        //print("Test ", BWHITE);
        //sleep(1000);

        //sleepPIT(1000);
        //sleepFor50000NOP(1000);
        /*for(int32 i = 0; i < 20; i++) {
            print("Test ", BWHITE);
        }*/

        // sleepFor50000NOP(5000);
        // clearScreen();

        //print("Test", BWHITE);
        //setBackgroundColor(RED);
        //sleepFor50000NOP(500);
        //clearScreen();
        //sleepPIT(1);

        //int8 key = readKey();
        //printchar(key, BWHITE);
    }
}