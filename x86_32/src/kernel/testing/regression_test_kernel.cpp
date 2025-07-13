#include <kernel/testing/regression_test_kernel.h>

#include <kernel/library/assert.h>
#include <kernel/library/string.h>

#include <kernel/drivers/video/vga.h>

#include <kernel/memory/mem.h>

#include <kernel/library/log.h>

namespace Testing {
    void testKernel () {
        testMemory();
        Library::fprintln("(PASSED) memory module", Drivers::VGA::Color::LGREEN);

        testString();
        Library::fprintln("(PASSED) string module", Drivers::VGA::Color::LGREEN);
    }

    void testMemory () {
        PTRMEM testAdd = (PTRMEM)0x100000;
        Memory::memset(testAdd, 69, 100);
        for(uint8 i = 0; i < 69; i++)
            Kernel::assert(testAdd[i] == 69, "memset error");
    }

    void testString () {
        //strlen
        cstr testStr = "Test str";
        uint16 strLen_testStr = Library::strlen(testStr);
        Kernel::assert(strLen_testStr == 8, "strlen error");

        //strcmp
        cstr testStrCmp1 = "String to compare 1";
        cstr testStrCmp2 = "String to compare 2";
        bool strcmpResult = Library::strcmp(testStrCmp1, testStrCmp2);
        Kernel::assert(!strcmpResult, "strcmp error");

        strcmpResult = Library::strcmp(testStrCmp1, testStrCmp1);
        Kernel::assert(strcmpResult, "strcmp error");

        //strcpy
        str testStrToCpy = (str)"String to copy";
        int8 targetString[STR_MAX_LEN];
        Library::strcpy(testStrToCpy, targetString);

        strcmpResult = Library::strcmp(testStrToCpy, targetString);
        Kernel::assert(strcmpResult, "strcpy error");
        
        //strcat
        cstr testStrToCat = "String 2";
        int8 outStrCat[STR_MAX_LEN] = "String 1 + ";
        Library::strcat((str)testStrToCat, outStrCat);

        strcmpResult = Library::strcmp("String 1 + String 2", outStrCat);
        Kernel::assert(strcmpResult, outStrCat);
    }
}