#include <kernel/memory/pmm.h>

#include <kernel/library/debug.h>
#include <kernel/library/panic.h>
#include <kernel/library/log.h>

#include <boot/limine/limine.h>

namespace Memory::Physical {

__attribute__((used,
               section(".limine_requests"))) volatile limine_hhdm_request hhdm_request = {.id = LIMINE_HHDM_REQUEST, .revision = 0, .response = nullptr};

static uint64_t mem_bitmap[MAX_INITIAL_PAGES];
 
PMMData pmm_data;

void init () {
    if(hhdm_request.response == NULLPTR)
        Kernel::panic("HHDM response was null");

    pmm_data.hhdm_offset = hhdm_request.response->offset;
    KLOG("HHDM offset is: %H", pmm_data.hhdm_offset);

    pmm_data.last_bitmap_index = 0;
}

PTR alloc_page() {
    for (uint64_t i = pmm_data.last_bitmap_index; i < MAX_INITIAL_PAGES; i++) {
        uint64_t cur_page = mem_bitmap[i];
        if((cur_page & 1) == 0) {
        }
    }
    return 0;
}

void free_page() {}
} // namespace Memory::Physical
