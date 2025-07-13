#pragma once

#include <utils/types.h>

#include <kernel/drivers/io/keyboard.h>

#define TERMINAL_BUFFER_SIZE    10

namespace Kernel::Terminal {
    void run();
    void handleKeypress (IN Drivers::Keyboard::KeypressInfo keypress);
    void displayCharacter(IN int8 _char);
    void newEntry ();
    void handleBuffer ();
}