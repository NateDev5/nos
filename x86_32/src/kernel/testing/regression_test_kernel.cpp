#include <kernel/testing/regression_test_kernel.h>

#include <kernel/debug/assert.h>

#include <kernel/memory/mem.h>

#include <kernel/drivers/video/vga.h>

namespace Testing {
    void testKernel () {
        testMemory();
        Drivers::VGA::println("(PASSED) memory module", LGREEN);
    }

    void testMemory () {
        PTRMEM testAdd = (PTRMEM)0x100000;
        Memory::memset(testAdd, 69, 100);
        for(uint8 i = 0; i < 69; i++)
            Debug::assert(testAdd[i] == 69, "memset error");
    }
}