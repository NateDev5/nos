#pragma once

#include <utils/types.h>

#include <kernel/drivers/io/keyboard.h>

#define KERNEL_SHELL_BUFFER_SIZE 20

#define IS_CMD(buffer, cmd)      Shared::strcmp(buffer, cmd) == 0

namespace Kernel::Shell {
void run_shell();
void handle_keypress(IN Drivers::Keyboard::KeypressInfo keypress);
void handle_command();
void new_entry();

void test_cmd();
void palloc_cmd();
void ansi_test_cmd();
void rect_cmd();
} // namespace Kernel::Shell
