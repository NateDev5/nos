#pragma once

#include <utils/types.h>

#define KiB_BOUNDS 0x400
#define MiB_BOUNDS 0x100000
#define GiB_BOUNDS 0x40000000
#define TiB_BOUNDS 0x10000000000
#define PiB_BOUNDS 0x4000000000000
#define EiB_BOUNDS 0x100000000000000

#define IN_RANGE(value, min, max) value >= min && value < max

namespace Shared::Humanize {
uint64_t bytes(IN uint64_t value, OUT CONST_CHAR_PTR& alias);
}
