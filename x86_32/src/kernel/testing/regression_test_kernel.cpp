#include <kernel/testing/regression_test_kernel.h>

#include <kernel/debug/assert.h>

#include <kernel/library/string.h>

#include <kernel/memory/mem.h>

#include <kernel/drivers/video/vga.h>

namespace Testing {
    void testKernel () {
        testMemory();
        Drivers::VGA::println("(PASSED) memory module", LGREEN);

        testString();
        Drivers::VGA::println("(PASSED) string module", LGREEN);
    }

    void testMemory () {
        PTRMEM testAdd = (PTRMEM)0x100000;
        Memory::memset(testAdd, 69, 100);
        for(uint8 i = 0; i < 69; i++)
            Debug::assert(testAdd[i] == 69, "memset error");
    }

    void testString () {
        //strlen
        cstr testStr = "Test str";
        uint16 strLen_testStr = Library::strlen(testStr);
        Debug::assert(strLen_testStr == 8, "strlen error");

        //strcmp
        cstr testStrCmp1 = "String to compare 1";
        cstr testStrCmp2 = "String to compare 2";
        bool strcmpResult = Library::strcmp(testStrCmp1, testStrCmp2);
        Debug::assert(!strcmpResult, "strcmp error");

        strcmpResult = Library::strcmp(testStrCmp1, testStrCmp1);
        Debug::assert(strcmpResult, "strcmp error");

        //strcpy
        str testStrToCpy = (str)"String to copy";
        str targetString;
        Library::strcpy(testStrToCpy, targetString);

        strcmpResult = Library::strcmp(testStrToCpy, targetString);
        Debug::assert(strcmpResult, "strcpy error");

        //strcat
        str testStrToCat1 = (str)"String 1 + ";
        str testStrToCat2 = (str)"String 2";
        int8 outStrCat[STR_MAX_LEN];
        Library::strcat(testStrToCat1, testStrToCat2, outStrCat);

        strcmpResult = Library::strcmp("String 1 + String 2", outStrCat);
        Debug::assert(strcmpResult, outStrCat);
    }
}