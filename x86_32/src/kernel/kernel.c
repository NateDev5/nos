#include "../memory/mem.h"
#include "../utils/math.h"
#include "../utils/timer.h"
#include "../video/video.h"
#include "../io/keyboard.h"

void kmain()
{
    // if (!initPS2Controller())
    //     println("Failed to init ps2 controller", LRED);

    while (true)
    {
        for(int32 i = 0; i < 20; i++) {
            sleepFor50000NOP(100);
            print("Test ", BWHITE);
        }

        sleepFor50000NOP(5000);
        clearScreen();
        // int8 key = readKey();
        // printchar(key, BWHITE);
    }
}