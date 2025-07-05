#include "keyboard.h"
#include "../video/video.h"
#include "../utils/math.h"
#include "io.h"

bool initPS2Controller()
{
    // perform self test
    // send 0xAA and check for 0x55 if its something other than that then the self test failed
    outb(PS2_STATUS_REGISTER, 0xAA);
    if (inb(PS2_DATA_PORT) != 0x55)
        return false;

    return true;
}

int8 readKey()
{
    uint8 ps2Status;
    do
    {
        ps2Status = inb(PS2_STATUS_REGISTER);
    } while (ps2Status == 0x1c);
    
    uint8 data = inb(PS2_DATA_PORT);

    if (data < 0x81)
        return scanCodes[data];

    return 0;
}