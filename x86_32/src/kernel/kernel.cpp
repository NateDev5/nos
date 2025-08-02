#include <kernel/testing/regression_test_kernel.h>

#include <kernel/drivers/hardware/pit.h>
#include <kernel/drivers/io/keyboard.h>
#include <kernel/drivers/serial/uart.h>
#include <kernel/drivers/video/vga.h>

#include <kernel/interrupts/idt.h>

#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/string.h>

#include <kernel/terminal.h>

extern "C" void kmain() {

    // for debugging
    Drivers::UART::init_port(COM_PORT_1);

    Drivers::VGA::disable_cursor();
    // Drivers::VGA::init();

    // testing
    Testing::test_kernel();

    // setup
    Drivers::Keyboard::init(); // before setting up interrupts

    Drivers::PIT::init();
    Interrupts::IDT::setup();

    // welcome
    Library::fprintln("Welcome to NOS!", Drivers::VGA::CYAN);

    // terminal
    Kernel::Terminal::run();
    // Drivers::VGA::offsetScreen();

    while (true) {
    }
}
