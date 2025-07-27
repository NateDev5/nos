#include <kernel/devices/io/ps2.h>

#include <kernel/library/assert.h>

#include <utils/asm.h>

namespace Devices::PS2 {
void init() {
    // perform self test
    outb(PS2_COMMAND_REGISTER, PS2_SELFTEST_SEND);
    ioWait();
    uint8_t self_test_result = inb(PS2_DATA_PORT);
    ASSERT(self_test_result == PS2_SELFTEST_RECEIVE, "PS2 Controller self test failed");

    // tell the controller the next byte written to data port is the keyboard
    // command byte
    outb(PS2_COMMAND_REGISTER, PS2_WRITE_COMMAND_BYTE);

    // set command byte (check docs for options)
    // disable translation to scan code set 1
    outb(PS2_DATA_PORT, 0x07); // 01010001
}

uint8_t poll() {
    uint8_t data = inb(PS2_DATA_PORT);

    // break code first byte, read the next to get the make code
    // if(data == 0xF0)
    //    data = inb(PS2_DATA_PORT);

    return data;
}
} // namespace Devices::PS2
