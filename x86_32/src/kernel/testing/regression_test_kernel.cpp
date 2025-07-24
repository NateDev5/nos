#include <kernel/testing/regression_test_kernel.h>

#include <kernel/library/assert.h>
#include <kernel/library/string.h>
#include <kernel/library/list.h>
#include <kernel/library/arrayutils.h>

#include <kernel/drivers/video/vga.h>

#include <kernel/memory/mem.h>

#include <kernel/library/log.h>

namespace Testing {
    void testKernel () {
        testMemory();
        Library::fprintln("(PASSED) memory test module", Drivers::VGA::Color::LGREEN);

        testString();
        Library::fprintln("(PASSED) string test module", Drivers::VGA::Color::LGREEN);

        testList();
        Library::fprintln("(PASSED) list test module", Drivers::VGA::Color::LGREEN);

        testArrayUtils();
        Library::fprintln("(PASSED) arrayutils test module", Drivers::VGA::Color::LGREEN);
    }

    void testMemory () {
        PTRMEM testAdd = (PTRMEM)0x100000;
        Memory::memset(testAdd, 69, 100);
        for(uint8 i = 0; i < 69; i++)
            Kernel::assert(testAdd[i] == 69, "memset error 1");
    }

    void testString () {
        //strlen
        cstr testStr = "Test str";
        uint16 strLen_testStr = Library::strlen(testStr);
        Kernel::assert(strLen_testStr == 8, "strlen error 1");

        //strcmp
        cstr testStrCmp1 = "String to compare 1";
        cstr testStrCmp2 = "String to compare 2";
        bool strcmpResult = Library::strcmp(testStrCmp1, testStrCmp2);
        Kernel::assert(!strcmpResult, "strcmp error 2");

        strcmpResult = Library::strcmp(testStrCmp1, testStrCmp1);
        Kernel::assert(strcmpResult, "strcmp error 3");

        //strcpy
        str testStrToCpy = (str)"String to copy";
        int8 targetString[STR_MAX_LEN];
        Library::strcpy(testStrToCpy, targetString);

        strcmpResult = Library::strcmp(testStrToCpy, targetString);
        Kernel::assert(strcmpResult, "strcpy error 4");
        
        //strcat
        cstr testStrToCat = "String 2";
        int8 outStrCat[STR_MAX_LEN] = "String 1 + ";
        Library::strcat((str)testStrToCat, outStrCat);

        strcmpResult = Library::strcmp("String 1 + String 2", outStrCat);
        Kernel::assert(strcmpResult, outStrCat);
    }

    void testList () {
        Library::List<int8, 20> list;
        
        Kernel::assert(list.length() == 0, "list error 1");
        Kernel::assert(list.maxSize() == 20, "list error 2");
        
        list.add('n');
        
        Kernel::assert(list.length() == 1, "list error 3");
        Kernel::assert(list[0] == 'n', "list error 4");
        
        list.add('O');
        list.add('S');
        
        Kernel::assert(list.length() == 3, "list error 5");
        Kernel::assert(list[1] == 'O', "list error 6");

        list.pop();
        Kernel::assert(list.length() == 2, "list error 7");
        Kernel::assert(list[1] == 'O', "list error 8");

        list[1] = 'B';
        Kernel::assert(list[1] == 'B', "list error 9");

        list.clear();
        Kernel::assert(list.length() == 0, "list error 10");
        Kernel::assert(list.maxSize() == 20, "list error 11");

        for(uint8 i = 0; i < 25; i++)
            list.add(i);

        Kernel::assert(list.length() == 20, "list error 12");
        Kernel::assert(list[0] == 0, "list error 13");
        Kernel::assert(list[19] == 19, "list error 14");

        list.clear();

        Kernel::assert(list.length() == 0, "list error 15");
        Kernel::assert(list.maxSize() == 20, "list error 16");

        list.add('A');
        list.add('B');
        list.add('C');

        list.removeAt(1);

        Kernel::assert(list.length() == 2, "list error 17");
        Kernel::assert(list[1] == 'C', "list error 18");

        list.add('A');
        list.add('B');
        list.add('C');
        list.add('D');
        list.add('E');

        list.removeAt(0);
        list.removeAt(4);
        list.removeAt(list.length() - 1);

        Kernel::assert(list.length() == 4, "list error 19");
        Kernel::assert(list[0] == 'C', "list error 20");
        Kernel::assert(list[1] == 'A', "list error 21");
        Kernel::assert(list[2] == 'B', "list error 22");
        Kernel::assert(list[3] == 'C', "list error 23");
    }

    void testArrayUtils () {
        uint8 testArrayLen = 20;
        int8 testArray [testArrayLen];
        uint8 testArrayPos = 0;

        for(uint8 i = 0; i < 10; i++) {
            testArray[i] = i;
            testArrayPos = i;
        }

        testArrayPos = Library::removeAt<int8>(testArray, testArrayLen, testArrayPos, 5);
        Kernel::assert(testArray[5] == 6, "arrayutils error 1");
    }
}