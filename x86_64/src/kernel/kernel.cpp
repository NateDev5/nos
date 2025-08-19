#include <kernel/library/panic.h>
#include <kernel/testing/regression_test_kernel.h>

#include <kernel/drivers/io/keyboard.h>
#include <kernel/drivers/serial/uart.h>
#include <kernel/drivers/video/video.h>

#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/sleep.h>
#include <kernel/library/string.h>

#include <boot/limine/limine_misc.h>

#include <kernel/arch/x86_64/gdt.h>
#include <kernel/arch/x86_64/interrupts/idt.h>
#include <kernel/arch/x86_64/timer/pit.h>

// #include <kernel/terminal.h>

extern "C" void kmain() {
    // for debugging
    Drivers::UART::init_port(COM_PORT_1);

    if (LIMINE_BASE_REVISION_SUPPORTED == false)
        Kernel::panic("Limine base revision not supported");

    Arch::x86_64::GDT::setup();

    Drivers::Keyboard::init(); // before setting up interrupts

    Arch::x86_64::PIT::init();
    Arch::x86_64::IDT::setup();

    // testing
    Testing::test_kernel();

    Drivers::Video::init();
    // DEBUG_PRINT("%i", result)
    //  welcome
    //  Library::fprintln("Welcome to NOS!", Drivers::VGA::CYAN);

    // terminal
    // Kernel::Terminal::run();
    // Drivers::VGA::offsetScreen();

    for (uint64_t x = 0; x < Drivers::Video::width(); x++) {
        for (uint64_t y = 0; y < Drivers::Video::height(); y++) {
            Drivers::Video::put_pixel(x, y, 0xFFFFFFFF);
            // Library::sleep(1);
        }
        Library::sleep(1);
    }

    while (true) {
    }
}
