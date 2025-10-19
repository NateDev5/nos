#include <kernel/memory/pmm.h>

#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/panic.h>

#include <shared/humanize.h>
#include <shared/mem.h>

#include <boot/limine/limine.h>

namespace Memory::Physical {

__attribute__((used,
               section(".limine_requests"))) volatile limine_memmap_request memmap_request = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0, .response = nullptr};

static uint64_t mem_bitmap[MAX_INITIAL_PAGES];

PMMData pmm_data;

void init() {
    if (memmap_request.response == NULLPTR)
        Kernel::panic("Memory map not found");

    KLOG("Memory map:")
    limine_memmap_response *memmap = memmap_request.response;
    CONST_CHAR_PTR          alias  = 0;
    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        limine_memmap_entry *entry           = memmap->entries[i];
        uint64_t             humanized_value = Shared::Humanize::bytes(entry->length, alias);
        KLOG("Base: %H, Length: %i %s (%i), Type: %i", entry->base, humanized_value, alias, entry->length, entry->type);
    }

    // pmm_data.hhdm_offset = hhdm_request.response->offset;
    // KLOG("HHDM offset is: %H", pmm_data.hhdm_offset);

    // pmm_data.last_bitmap_index = 0;
}

PTR alloc_page() {
    uint64_t page_index = pmm_data.last_bitmap_index;
    for (; page_index < MAX_INITIAL_PAGES; page_index++) {
        uint64_t cur_page = mem_bitmap[page_index];
        if ((cur_page & 1) == 0) {
            mem_bitmap[page_index] |= 1;
            break;
        }
    }
    uint64_t page_addr = ((page_index * 64) + 8) * PAGE_SIZE;
    Shared::memset((PTR)page_addr, 0, PAGE_SIZE);
    KLOG("Page address: %H", page_addr);
    return (PTR)page_addr;
}

void free_page() {}
} // namespace Memory::Physical
