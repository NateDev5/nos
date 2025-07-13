#include <kernel/testing/regression_test_kernel.h>

#include <kernel/drivers/io/keyboard.h>
#include <kernel/drivers/hardware/pit.h>
#include <kernel/drivers/video/vga.h>

#include <kernel/interrupts/idt.h>

#include <kernel/library/string.h>
#include <kernel/library/log.h>

#include <kernel/terminal.h>

extern "C" void kmain()
{
    Drivers::VGA::disableCursor();

    // testing
    Testing::testKernel();

    // setup
    Drivers::Keyboard::init(true); // before setting up interrupts

    Drivers::PIT::init(true);
    Interrupts::IDT::setup(true);

    // welcome
    Library::fprintln("Welcome to NOS!", Drivers::VGA::CYAN);

    // terminal
    Kernel::Terminal::run();
   
    while (true)
    {
    }
}