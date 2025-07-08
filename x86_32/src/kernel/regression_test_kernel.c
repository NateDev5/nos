#include "regression_test_kernel.h"

#include "../utils/assert.h"
#include "../memory/mem.h"
#include "../video/vga.h"

void testKernel () {
    testMemory();
    println("(PASSED) memory module", LGREEN);
}

void testMemory () {
    PTRMEM testAdd = (PTRMEM)0x100000;
    memset(testAdd, 69, 100);
    for(uint8 i = 0; i < 69; i++)
        assert(testAdd[i] == 69, "memset error");
}