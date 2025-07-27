// https://oe7twj.at/images/6/6a/PS2_Keyboard.pdf
// https://wiki.osdev.org/PS/2_Keyboard

#pragma once

#include <utils/types.h>

#define PS2_DATA_PORT          0x60
#define PS2_STATUS_REGISTER    0x64
#define PS2_COMMAND_REGISTER   0x64

#define PS2_SELFTEST_SEND      0xAA
#define PS2_SELFTEST_RECEIVE   0x55

#define PS2_ACK                0xFA
#define PS2_RESEND             0xFE

#define PS2_READ_COMMAND_BYTE  0x20
#define PS2_WRITE_COMMAND_BYTE 0x60

namespace Devices::PS2 {
void    init();
uint8_t poll();
} // namespace Devices::PS2
