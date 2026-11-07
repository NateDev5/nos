#define IA32_APIC_BASE_MSR_REGISTER 0x1B
#define LVT_LINT0_REGISTER 0x350

namespace Arch::x86_64::LAPIC {
	void unmask_lint0 ();
}
