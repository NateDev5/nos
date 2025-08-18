#pragma once

#include <utils/types.h>

namespace Math {
CHAR_PTR itoa(IN int64_t value, OUT CHAR_PTR outStr, IN uint8_t base, IN bool caps = false, IN bool include_zeros = false);
}
