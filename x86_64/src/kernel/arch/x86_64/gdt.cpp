#include <kernel/arch/x86_64/gdt.h>

#include <kernel/library/debug.h>

#include <utils/asm.h>

extern "C" void load_gdt(IN Arch::x86_64::GDT::GDTR *gdtr);

namespace Arch::x86_64::GDT {
static GDT_ENTRY gdt[GDT_SIZE];
GDTR             gdtr;
void             setup() {
    cli();
    set_gdt_entry(0, 0, 0, 0, 0);            // null descriptor
    set_gdt_entry(1, 0, 0xFFFF, 0x9A, 0xAF); // kernel code segment
    set_gdt_entry(2, 0, 0xFFFF, 0x92, 0xCF); // kernel data segment
    set_gdt_entry(3, 0, 0xFFFF, 0xFA, 0xAF); // user code segment
    set_gdt_entry(4, 0, 0xFFFF, 0xF2, 0xCF); // user data segment

    gdtr.base = (uint64_t)&gdt[0];
    gdtr.size = sizeof(GDT_ENTRY) * GDT_SIZE - 1;

    load_gdt(&gdtr);

    DEBUG_RAW("(OK) GDT initialized")
}

void set_gdt_entry(IN uint8_t index, IN uint32_t base, IN uint16_t limit, IN uint8_t access, IN uint8_t flags) {
    GDT_ENTRY *entry = &gdt[index];
    // in 64 bit, base and limit value are ignored
    entry->limit     = limit & 0xFFFF;
    entry->base_low  = base & 0xFFFF;
    entry->base_med  = (base >> 16) & 0xFF;
    entry->base_high = (base >> 24) & 0xFF;
    entry->access    = access;
    entry->flags     = flags;
}

} // namespace Arch::x86_64::GDT
