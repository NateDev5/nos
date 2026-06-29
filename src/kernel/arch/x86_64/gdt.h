#pragma once

#include <utils/types.h>

#define GDT_SIZE 5

namespace Arch::x86_64::GDT {
struct GDTR {
    uint16_t size;
    uint64_t base;
} __attribute__((packed));

struct GDT_ENTRY {
    uint16_t limit;
    uint16_t base_low;
    uint8_t  base_med;
    uint8_t  access;
    uint8_t  flags;
    uint8_t  base_high;
} __attribute__((packed));

void setup();
void set_gdt_entry(IN uint8_t index, IN uint32_t base, IN uint16_t limit, IN uint8_t access, uint8_t flags);
} // namespace Arch::x86_64::GDT
