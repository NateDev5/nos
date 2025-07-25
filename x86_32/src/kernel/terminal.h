#pragma once

#include <utils/types.h>

#include <kernel/drivers/io/keyboard.h>

#define TERMINAL_BUFFER_SIZE    256

namespace Kernel::Terminal {
    void run();
    void handle_keypress (IN Drivers::Keyboard::KeypressInfo keypress);
    void display_character(IN int8 _char);
    void new_entry ();
    void handle_buffer ();
}
