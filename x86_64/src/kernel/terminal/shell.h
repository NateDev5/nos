#pragma once

#include <utils/types.h>

#include <kernel/drivers/io/keyboard.h>

namespace Kernel::Shell {
void run_shell ();
void handle_keypress(IN Drivers::Keyboard::KeypressInfo keypress);
void new_entry();
}
