#pragma once

#include <utils/types.h>

namespace Memory {
void memset(IN PTR address, IN uint8_t data, IN uint64_t size);
void memcpy(IN PTR base_address, IN PTR target_address, IN uint64_t size, IN bool reversed = false);
} // namespace Memory
