#pragma once

#include <utils/types.h>

namespace Kernel {
void panic(IN CONST_CHAR_PTR msg);
void error_screen(IN CONST_CHAR_PTR error_msg);
} // namespace Kernel
