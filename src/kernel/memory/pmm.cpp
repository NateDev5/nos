#include <kernel/memory/pmm.h>

#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/panic.h>

#include <shared/humanize.h>
#include <shared/mem.h>

#include <boot/limine/limine.h>

namespace Memory::Physical {

__attribute__((
    used, section(".limine_requests"))) volatile limine_memmap_request memmap_request = {.id = LIMINE_MEMMAP_REQUEST_ID, .revision = 0, .response = nullptr};


static uint64_t mem_bitmap[BITMAP_SIZE];

PMMData pmm_data;

CONST_CHAR_PTR memmap_entry_types[9] = {"\033[0;32mUsable\033[0m",
                                        "\033[0;31mReserved\033[0m",
                                        "\033[0;32mACPI Reclaimable\033[0m",
                                        "ACPI Nvs",
                                        "\033[0;31mBad Memory\033[0m",
                                        "\033[0;32mBootloader Reclaimable\033[0m",
                                        "Executable And Modules",
                                        "Framebuffer",
                                        "ACPI Tables"};

void init() {
    if (memmap_request.response == nullptr) {
        Kernel::panic("Memory map not found");
    }

    KINFO("Memory map:")
    limine_memmap_response *memmap = memmap_request.response;
    CONST_CHAR_PTR          alias  = 0;
    for (uint64_t i = 0; i < memmap->entry_count; i++) {
        limine_memmap_entry *entry = memmap->entries[i];

        uint64_t humanized_value = Shared::Humanize::bytes(entry->length, alias);
        KINFO("Base: %H, Length: %i %s (%i), Type: %s", entry->base, humanized_value, alias, entry->length, memmap_entry_types[entry->type]);

        if (entry->base != 0x000000FD00000000 && (entry->type == LIMINE_MEMMAP_RESERVED || entry->type == LIMINE_MEMMAP_EXECUTABLE_AND_MODULES ||
                                                  entry->type == LIMINE_MEMMAP_FRAMEBUFFER || entry->type == LIMINE_MEMMAP_ACPI_NVS)) {
            uint32_t num_of_pages = entry->length / PAGE_SIZE;

            for (size_t i = 0; i < num_of_pages; i++) {
                uint64_t page_index = ((entry->base + (i * PAGE_SIZE)) - BITMAP_START_ADDRESS) / PAGE_SIZE;
                uint64_t bitmap_index =
                    page_index /
                    PAGES_PER_BITMAP_ENTRY; // since each bit = page (uint64_t can hold 64 bit so 64 pages). this finds in which "row" the pages belong
                uint64_t bit_offset      = page_index % PAGES_PER_BITMAP_ENTRY;
                mem_bitmap[bitmap_index] = mem_bitmap[bitmap_index] | ((uint64_t)1 << bit_offset);
            }
        }
    }

    pmm_data.last_bitmap_index = 0;
    pmm_data.last_bit_index    = 0;

    KINFO("PMM Initialized")
}

PTR alloc_page() {
    while (pmm_data.last_bitmap_index < BITMAP_SIZE) {
        uint64_t cur_bitmap = mem_bitmap[pmm_data.last_bitmap_index];
        while ((cur_bitmap & ((uint64_t)1 << pmm_data.last_bit_index)) != 0) {
            pmm_data.last_bit_index++;
            if (pmm_data.last_bit_index == PAGES_PER_BITMAP_ENTRY) {
                pmm_data.last_bit_index = 0;
                pmm_data.last_bitmap_index++;
            }

            if (pmm_data.last_bitmap_index == BITMAP_SIZE) {
                KFATAL("No pages left to alloc");
                return nullptr;
            }
        }
        mem_bitmap[pmm_data.last_bitmap_index] = mem_bitmap[pmm_data.last_bitmap_index] | ((uint64_t)1 << pmm_data.last_bit_index);
        uint64_t page_index                    = pmm_data.last_bitmap_index * PAGES_PER_BITMAP_ENTRY + pmm_data.last_bit_index;
        uint64_t addr                          = page_index * PAGE_SIZE + BITMAP_START_ADDRESS;
        return (PTR)addr;
    }
    return nullptr;
}

void free_page() {}
} // namespace Memory::Physical
