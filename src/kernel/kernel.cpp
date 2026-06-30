#include <kernel/library/panic.h>

#include <kernel/drivers/io/keyboard.h>
#include <kernel/drivers/serial/uart.h>
#include <kernel/drivers/video/framebuffer.h>

#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/sleep.h>

#include <boot/limine/limine_misc.h>

#include <kernel/arch/x86_64/gdt.h>
#include <kernel/arch/x86_64/interrupts/idt.h>
#include <kernel/arch/x86_64/timer/pit.h>

#include <kernel/terminal/shell.h>
#include <kernel/terminal/term.h>

#include <kernel/testing/tests.h>
#include <utils/asm.h>
#include <kernel/memory/pmm.h>

#include <shared/tests.h>

extern "C" void kmain() {
    // for debugging
    Drivers::UART::init_port(COM_PORT_1);

    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
        Kernel::panic("Limine base revision not supported");

    Arch::x86_64::GDT::setup();

    // interrupts
    Arch::x86_64::IDT::setup();
    Arch::x86_64::PIT::init();
	sti();
    
	Drivers::Video::Framebuffer::init();
    Kernel::Terminal::init();

    // input
    Drivers::Keyboard::init(); // before setting up interrupts


	// memory
    Memory::Physical::init();
    
	// testing
#ifdef __TESTING__
    Kernel::Tests::test_group_kernel();
    Shared::Tests::test_group_shared_lib();
#endif
    // shell
    Library::println("Starting shell...");
	Library::sleep(100);
	Kernel::Shell::run_shell();

    while (true) {
    }
}
