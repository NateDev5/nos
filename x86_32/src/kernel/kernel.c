#include "../memory/mem.h"
#include "../utils/math.h"
#include "../utils/timer.h"
#include "../video/video.h"
#include "../io/keyboard.h"
#include "../interrupts/idt.h"
#include "../utils/asm.h"

void kmain()
{
    setupIDT();
    // if (!initPS2Controller())
    //     println("Failed to init ps2 controller", LRED);

    int i = 0/0;

    //__asm__ volatile("int $0x1");
    
    while (true)
    {
        //print("Test ", BWHITE);
        //sleepPIT(10);
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