#include <boot/limine/limine.h>
#include <kernel/library/debug.h>
#include <kernel/library/panic.h>
#include <kernel/arch/x86_64/interrupts/lapic.h>
#include <utils/types.h>

namespace Arch::x86_64::LAPIC {

void unmask_lint0() {
    uint32_t addr_high;
    uint32_t addr_low;
    __asm__ volatile("rdmsr" : "=d"(addr_high), "=a"(addr_low) : "c"(IA32_APIC_BASE_MSR_REGISTER));

    uint64_t ia32_apic_base = ((uint64_t)addr_high << 32) | addr_low;
	ia32_apic_base &= ~0xFFF;
	DEBUG_INFO("IA32_APIC_BASE: %H", ia32_apic_base)


	volatile uint32_t* lvt_lint0_register_addr = (volatile uint32_t*)(ia32_apic_base + LVT_LINT0_REGISTER);
	DEBUG_INFO("LVT LINT0 Register addr: %H", lvt_lint0_register_addr)

	Kernel::panic("Finish PMM & VMM and map the LAPIC MMIO page to be able to modify the lvt entry to make hardware interrupts work\nRedo logging and itoa function because when using %H is flips the values");

	//uint32_t reg_val = *lvt_lint0_register_addr;
	//DEBUG_INFO("%h", *lvt_lint0_register_addr)
}
} // namespace Arch::x86_64::LAPIC
