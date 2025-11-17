#include <kernel/arch/x86_64/time/time.h>

namespace Arch::x86_64::Time {
uint64_t get_timestamp_counter() {
    uint32_t edx_value;
	uint32_t eax_value;

	__asm__ volatile("rdtsc");
	__asm__ volatile("mov %%edx, %0" : "=r"(edx_value));
	__asm__ volatile("mov %%eax, %0" : "=r"(eax_value));

	uint64_t timestamp_value = (edx_value >> 31) | eax_value;
	return timestamp_value;
}
} // namespace Arch::x86_64::Time
