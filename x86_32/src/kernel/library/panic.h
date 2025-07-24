#pragma once

#include <utils/types.h>

namespace Kernel {
    void panic (IN cstr msg);
    void errorScreen (IN cstr errorMsg);
}