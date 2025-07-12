/*#include "../memory/mem.h"
#include "../utils/math.h"
#include "../video/vga.h"
#include "../io/keyboard.h"
#include "../interrupts/idt.h"
#include "../utils/asm.h"
#include "../utils/pit.h"
#include "regression_test_kernel.h"*/

#include <kernel/testing/regression_test_kernel.h>

#include <kernel/drivers/io/keyboard.h>
#include <kernel/drivers/hardware/pit.h>
#include <kernel/drivers/video/vga.h>

#include <kernel/interrupts/idt.h>

extern "C" void kmain()
{
    Testing::testKernel();

    Drivers::Keyboard::init(true); // before setting up interrupts

    Drivers::PIT::init(true);
    Interrupts::IDT::setup(true);

    Drivers::VGA::disableCursor();
   
    while (true)
    {
    }
}