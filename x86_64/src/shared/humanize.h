#pragma once

#include <utils/types.h>

#define KB_BOUNDS 0x400
#define MB_BOUNDS 0x100000
#define GB_BOUNDS 0x40000000
#define TB_BOUNDS 0x10000000000
#define PB_BOUNDS 0x4000000000000
#define EB_BOUNDS 0x100000000000000

#define IN_RANGE(value, min, max) value >= min && value < max

namespace Shared::Humanize {
uint64_t bytes(IN uint64_t value, OUT CONST_CHAR_PTR& alias);
}
