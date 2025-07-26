#pragma once

#include <utils/types.h>

#define COM_PORT_1  0x3F8
#define COM_PORT_2  0x2F8
#define COM_PORT_3  0x3E8
#define COM_PORT_4  0x2E8
#define COM_PORT_5  0x5F8
#define COM_PORT_6  0x4F8
#define COM_PORT_7  0x5E8
#define COM_PORT_8  0x4E8

namespace Drivers::UART {
void init_port (IN uint16 port); 
}
