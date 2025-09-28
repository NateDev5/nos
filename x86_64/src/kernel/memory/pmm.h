// https://github.com/dreamportdev/Osdev-Notes/blob/master/04_Memory_Management/02_Physical_Memoriy.md
// https://blog.pepsipu.com/posts/pmm-for-oses
// http://www.brokenthorn.com/Resources/OSDev17.html
// https://wiki.osdev.org/Memory_Allocation#A_very_very_simple_Memory_Manager
/*
 * The physical memory manager is responsible for keeping track of used/free physical memory maps
 */

#pragma once

#include <utils/types.h>

#define PAGE_SIZE 4096 // 4kb

#define MAX_PAGES 65562

namespace Memory::Physical {
PTR alloc_page (uint64_t size);
void free_page();
}
