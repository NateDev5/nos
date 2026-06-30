// https://github.com/dreamportdev/Osdev-Notes/blob/master/04_Memory_Management/02_Physical_Memoriy.md
// https://blog.pepsipu.com/posts/pmm-for-oses
// http://www.brokenthorn.com/Resources/OSDev17.html
// https://wiki.osdev.org/Memory_Allocation#A_very_very_simple_Memory_Manager

#pragma once

#include <utils/types.h>

#define PAGE_SIZE              4096 // 4kb

#define MAX_INITIAL_PAGES      65562 // uint16_t max
#define PAGES_PER_BITMAP_ENTRY 64
#define BITMAP_SIZE            MAX_INITIAL_PAGES / PAGES_PER_BITMAP_ENTRY
#define BITMAP_START_ADDRESS   0x1000 // where the first page is located

namespace Memory::Physical {
struct PMMData {
    uint64_t last_bitmap_index;
    uint64_t last_bit_index;
};

void init();
PTR  alloc_page();
void free_page();
} // namespace Memory::Physical
