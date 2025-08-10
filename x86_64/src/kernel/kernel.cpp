#include "kernel/library/panic.h"
#include "utils/asm.h"
#include <kernel/testing/regression_test_kernel.h>

#include <kernel/drivers/hardware/pit.h>
#include <kernel/drivers/io/keyboard.h>
#include <kernel/drivers/serial/uart.h>

#include <kernel/interrupts/idt.h>

#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/string.h>

#include <boot/limine/limine_misc.h>

// #include <kernel/terminal.h>

extern "C" void kmain() {
    // for debugging
    Drivers::UART::init_port(COM_PORT_1);

    if (LIMINE_BASE_REVISION_SUPPORTED == false)
        Kernel::panic("Limine base revision not supported");

    if (framebuffer_request.response == NULLPTR || framebuffer_request.response->framebuffer_count < 1)
        Kernel::panic("Failed to get a framebuffer");

    // limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // testing
    // Testing::test_kernel();

    // setup
    // Drivers::Keyboard::init(); // before setting up interrupts

    Drivers::PIT::init();
    Interrupts::IDT::setup();

    // welcome
    // Library::fprintln("Welcome to NOS!", Drivers::VGA::CYAN);

    // terminal
    // Kernel::Terminal::run();
    // Drivers::VGA::offsetScreen();

    while (true)
        ;
}
