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

#include <boot/bootloader.h>

extern "C" void kmain(const BootloaderInfo* bootloader_info) {
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

    // DEBUG_PRINT("Struct ptr: %h\nBase addr: %h\nLength: %i\nType: %i", test, test->base_addr_low, test->length_low, test->type)

    for(uint16_t i = 0; i < 100; i++) {
        MemoryMapEntry entry = bootloader_info->mmap_entries[i];
        DEBUG_PRINT("Base addr: %h\nLength: %i\nType: %i",entry.base_addr_low, entry.length_low, entry.type)
    }

    DEBUG_PRINT("%h", bootloader_info);

    // terminal
    Kernel::Terminal::run();
    // Drivers::VGA::offsetScreen();

    while (true);
}
