#pragma once

#include <utils/types.h>

namespace Memory {
void memset(IN PTRMEM address, IN uint8_t data, IN uint32_t size);
void memcpy(IN PTRMEM base_address, IN PTRMEM target_address, IN uint32_t size);
} // namespace Memory
