#include <kernel/testing/regression_test_kernel.h>

#include <kernel/library/assert.h>
#include <kernel/library/string.h>
#include <kernel/library/list.h>
#include <kernel/library/arrayutils.h>

#include <kernel/drivers/video/vga.h>

#include <kernel/memory/mem.h>

#include <kernel/library/log.h>

namespace Testing {
    void test_kernel () {
        test_memory();
        Library::fprintln("(PASSED) memory test module", Drivers::VGA::Color::LGREEN);

        test_string();
        Library::fprintln("(PASSED) string test module", Drivers::VGA::Color::LGREEN);

        test_list();
        Library::fprintln("(PASSED) list test module", Drivers::VGA::Color::LGREEN);

        test_array_utils();
        Library::fprintln("(PASSED) arrayutils test module", Drivers::VGA::Color::LGREEN);
    }

    void test_memory () {
        PTRMEM test_add = (PTRMEM)0x100000;
        Memory::memset(test_add, 69, 100);
        for(uint8 i = 0; i < 69; i++)
            Kernel::assert(test_add[i] == 69, "memset error 1");
    }

    void test_string () {
        //strlen
        cstr test_str = "Test str";
        uint16 str_len_test_str = Library::strlen(test_str);
        Kernel::assert(str_len_test_str == 8, "strlen error 1");

        //strcmp
        cstr test_str_cmp1 = "String to compare 1";
        cstr test_str_cmp2 = "String to compare 2";
        bool str_cmp_result = Library::strcmp(test_str_cmp1, test_str_cmp2);
        Kernel::assert(!str_cmp_result, "strcmp error 2");

        str_cmp_result = Library::strcmp(test_str_cmp1, test_str_cmp1);
        Kernel::assert(str_cmp_result, "strcmp error 3");

        //strcpy
        str test_str_to_cpy = (str)"String to copy";
        int8 target_string[STR_MAX_LEN];
        Library::strcpy(test_str_to_cpy, target_string);

        str_cmp_result = Library::strcmp(test_str_to_cpy, target_string);
        Kernel::assert(str_cmp_result, "strcpy error 4");
        
        //strcat
        cstr test_str_to_cat = "String 2";
        int8 out_str_cat[STR_MAX_LEN] = "String 1 + ";
        Library::strcat((str)test_str_to_cat, out_str_cat);

        str_cmp_result = Library::strcmp("String 1 + String 2", out_str_cat);
        Kernel::assert(str_cmp_result, out_str_cat);
    }

    void test_list () {
        Library::List<int8, 20> list;
        
        Kernel::assert(list.length() == 0, "list error 1");
        Kernel::assert(list.max_size() == 20, "list error 2");
        
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
        Kernel::assert(list.max_size() == 20, "list error 11");

        for(uint8 i = 0; i < 25; i++)
            list.add(i);

        Kernel::assert(list.length() == 20, "list error 12");
        Kernel::assert(list[0] == 0, "list error 13");
        Kernel::assert(list[19] == 19, "list error 14");

        list.clear();

        Kernel::assert(list.length() == 0, "list error 15");
        Kernel::assert(list.max_size() == 20, "list error 16");

        list.add('A');
        list.add('B');
        list.add('C');

        list.remove_at(1);

        Kernel::assert(list.length() == 2, "list error 17");
        Kernel::assert(list[1] == 'C', "list error 18");

        list.add('A');
        list.add('B');
        list.add('C');
        list.add('D');
        list.add('E');

        list.remove_at(0);
        list.remove_at(4);
        list.remove_at(list.length() - 1);

        Kernel::assert(list.length() == 4, "list error 19");
        Kernel::assert(list[0] == 'C', "list error 20");
        Kernel::assert(list[1] == 'A', "list error 21");
        Kernel::assert(list[2] == 'B', "list error 22");
        Kernel::assert(list[3] == 'C', "list error 23");
    }

    void test_array_utils () {
        uint8 test_array_len = 20;
        int8 test_array [test_array_len];
        uint8 test_array_pos = 0;

        for(uint8 i = 0; i < 10; i++) {
            test_array[i] = i;
            test_array_pos = i;
        }

        test_array_pos = Library::remove_at<int8>(test_array, test_array_len, test_array_pos, 5);
        Kernel::assert(test_array[5] == 6, "arrayutils error 1");
    }
}
