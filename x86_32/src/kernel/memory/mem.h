#pragma once

#include <utils/types.h>

namespace Memory {
    void memset (IN PTRMEM address, IN uint8 data, IN uint32 size);
    void memcpy (IN PTRMEM base_address, IN PTRMEM target_address, IN uint32 size);
}
