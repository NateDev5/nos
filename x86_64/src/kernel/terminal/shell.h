#pragma once

#include <utils/types.h>

#include <kernel/drivers/io/keyboard.h>

#define KERNEL_SHELL_BUFFER_SIZE 20

namespace Kernel::Shell {
void run_shell ();
void handle_keypress(IN Drivers::Keyboard::KeypressInfo keypress);
void handle_command ();
void new_entry();

void test_cmd ();
}
