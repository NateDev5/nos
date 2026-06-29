#include <kernel/library/sleep.h>

#include <kernel/arch/x86_64/timer/pit.h>

namespace Library {
void sleep(IN uint64_t milliseconds) { Arch::x86_64::PIT::sleep(milliseconds); }
} // namespace Library
