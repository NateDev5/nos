#pragma once

#include <utils/types.h>

struct MemoryMapEntry {
    uint32_t base_addr_low;
    uint32_t base_addr_high;

    uint32_t length_low;
    uint32_t length_high;

    uint32_t type;
} __attribute__((packed));

struct BootloaderInfo {
    MemoryMapEntry* mmap_entries;
};
