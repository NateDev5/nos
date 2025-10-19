// https://github.com/dreamportdev/Osdev-Notes/blob/master/04_Memory_Management/02_Physical_Memoriy.md
// https://blog.pepsipu.com/posts/pmm-for-oses
// http://www.brokenthorn.com/Resources/OSDev17.html
// https://wiki.osdev.org/Memory_Allocation#A_very_very_simple_Memory_Manager

#pragma once

#include <utils/types.h>

#define PAGE_SIZE         4096 // 4kb

#define MAX_INITIAL_PAGES 65562 // uint16_t max

namespace Memory::Physical {
struct PMMData {
    uint64_t hhdm_offset;
    uint64_t last_bitmap_index;
};

void init();
PTR  alloc_page();
void free_page();
} // namespace Memory::Physical
