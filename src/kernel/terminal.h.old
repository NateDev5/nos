#pragma once

#include <utils/types.h>

#include <kernel/drivers/io/keyboard.h>

#define TERMINAL_BUFFER_SIZE 30

namespace Kernel::Terminal {
void run();
void handle_keypress(IN Drivers::Keyboard::KeypressInfo keypress);
void display_character(IN char _char);
void new_entry();
void handle_buffer();
} // namespace Kernel::Terminal
