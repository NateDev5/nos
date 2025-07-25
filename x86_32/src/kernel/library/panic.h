#pragma once

#include <utils/types.h>

namespace Kernel {
    void panic (IN cstr msg);
    void error_screen (IN cstr error_msg);
}
