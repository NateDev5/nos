#include <kernel/memory/pmm.h>

#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/panic.h>

#include <shared/humanize.h>
#include <shared/mem.h>

#include <boot/limine/limine.h>

namespace Memory::Physical {

__attribute__((used, section(".limine_requests"))) volatile limine_hhdm_request hhdm_request = {.id = LIMINE_HHDM_REQUEST, .revision = 0, .response = nullptr};

// static uint64_t mem_bitmap[MAX_INITIAL_PAGES];

PMMData pmm_data;

/*
CONST_CHAR_PTR memmap_entry_types[9] = {
        "\033[0;32mUsable\033[0m",
        "\033[0;31mReserved\033[0m",
        "\033[0;32mACPI Reclaimable\033[0m",
        "ACPI Nvs",
        "\033[0;31mBad Memory\033[0m",
        "\033[0;32mBootloader Reclaimable\033[0m",
        "Executable And Modules",
        "Framebuffer",
        "ACPI Tables"
};
*/

void init() {
    if (hhdm_request.response == nullptr)
        Kernel::panic("HHDM not found");

    /*
KINFO("Memory map:")
limine_memmap_response *memmap = memmap_request.response;
CONST_CHAR_PTR          alias  = 0;
for (uint64_t i = 0; i < memmap->entry_count; i++) {
    limine_memmap_entry *entry           = memmap->entries[i];
    uint64_t             humanized_value = Shared::Humanize::bytes(entry->length, alias);
    KINFO("Base: %H, Length: %i %s (%i), Type: %s", entry->base, humanized_value, alias, entry->length, memmap_entry_types[entry->type]);
            if(pmm_data.test_offset == 0 && entry->type == LIMINE_MEMMAP_USABLE)
                    pmm_data.test_offset = entry->base;
    }
    */

    // pmm_data.hhdm_offset = hhdm_request.response->offset;
    // KLOG("HHDM offset is: %H", pmm_data.hhdm_offset);

    pmm_data.hhdm_offset       = hhdm_request.response->offset;
    pmm_data.last_bitmap_index = 0;
}

PTR alloc_page() {
	/*
	while(pmm_data.last_bitmap_index < MAX_INITIAL_PAGES) {
		uint64_t cur_bitmap = mem_bitmap[pmm_data.last_bitmap_index];

		if(cur_bitmap == 0) {
		}
	}

	KINFO("Page index: %i, Bit index: %i, Bitmap: %b", page_index, bit_index, mem_bitmap[page_index]);

    uint64_t page_addr = (((page_index * bit_index) + 8) * PAGE_SIZE) + pmm_data.hhdm_offset;
    Shared::memset((PTR)page_addr, 0, PAGE_SIZE);
    KINFO("Page address: %H", page_addr);
    return (PTR)page_addr;
*/
	return 0;
	}

void free_page() {}
} // namespace Memory::Physical
