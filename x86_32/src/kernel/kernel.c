#include "../video/video.h"
#include "../memory/mem.h"
#include "../utils/math.h"
#include "../io/keyboard.h"

void kmain()
{
    //print("Hello world from NOS!\nLets goo!!!\n", PRNT_FORMAT(GREEN, BWHITE));
    if(!initKeyboard())
        println("Failed to initialize serial port 1", LRED);
    else println("Successfully initialized serial port 1", LGREEN);

    while(true) {}
}