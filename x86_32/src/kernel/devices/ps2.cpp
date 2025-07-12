#include <kernel/devices/ps2.h>

#include <kernel/debug/assert.h>

#include <utils/asm.h>

static uint8 currentFlags = 0;

namespace Devices::PS2 {
    void init () {
        // perform self test
        outb(PS2_COMMAND_REGISTER, PS2_SELFTEST_SEND);
        ioWait();
        uint8 selfTestResult = inb(PS2_DATA_PORT);
        Debug::assert(selfTestResult == PS2_SELFTEST_RECEIVE, "PS2 Controller self test failed");

        // tell the controller the next byte written to data port is the keyboard command byte
        outb(PS2_COMMAND_REGISTER, PS2_WRITE_COMMAND_BYTE);

        // set command byte (check docs for options)
        // disable translation to scan code set 1
        outb(PS2_DATA_PORT, 0x07); // 01010001
    }

    uint8 poll () {
        uint8 data = inb(PS2_DATA_PORT);

        // break code first byte, read the next to get the make code
        //if(data == 0xF0) 
        //    data = inb(PS2_DATA_PORT);

        return data;
    }
}