#include <kernel/testing/regression_test_kernel.h>

#include <kernel/library/arrayutils.h>
#include <kernel/library/assert.h>
// #include <kernel/library/list.h>
#include <kernel/library/string.h>

#include <kernel/drivers/video/vga.h>

#include <kernel/memory/mem.h>

#include <kernel/library/log.h>

namespace Testing {
void test_kernel() {
    test_memory();
    KLOG("(PASSED) memory test module")

    test_string();
    KLOG("(PASSED) string test module")

    // test_list();
    // DEBUG_PRINT("(PASSED) list test module")

    test_array_utils();
    KLOG("(PASSED) arrayutils test module")
}

void test_memory() {
    /*PTRMEM test_add = (PTRMEM)0x100000;
    Memory::memset(test_add, 69, 100);
    for (uint8_t i = 0; i < 69; i++)
        ASSERT_FATAL(test_add[i] == 69, "memset error 1")

    PTRMEM test_cpy = test_add + 100;
    Memory::memcpy(test_add, test_cpy, 69);

    for (uint8_t i = 0; i < 69; i++)
        ASSERT_FATAL(test_cpy[i] == 69, "memcpy error 1")

    for (uint8_t i = 0; i < 69; i++)
        test_cpy[i] = i;

    Memory::memcpy(test_cpy, (test_cpy + 2), 69, true);
    test_cpy[0] = 69;
    test_cpy[1] = 69;

    ASSERT_FATAL(test_cpy[0] == 69, "memcpy reversed error 1")
    ASSERT_FATAL(test_cpy[2] == 0, "memcpy reversed error 2")
    ASSERT_FATAL(test_cpy[7] == 5, "memcpy reversed error 3")
    */
}

void test_string() {
    // strlen
    CONST_CHAR_PTR test_str         = "Test str";
    uint32_t       str_len_test_str = Library::strlen(test_str);
    ASSERT_FATAL(str_len_test_str == 8, "strlen error 1")

    // strcmp
    CONST_CHAR_PTR test_str_cmp1  = "String to compare 1";
    CONST_CHAR_PTR test_str_cmp2  = "String to compare 2";
    int32_t        str_cmp_result = Library::strcmp(test_str_cmp1, test_str_cmp2);
    ASSERT_FATAL(str_cmp_result < 0, "strcmp error 1")

    str_cmp_result = Library::strcmp(test_str_cmp1, test_str_cmp1);
    ASSERT_FATAL(str_cmp_result == 0, "strcmp error 2")

    // strcpy
    CHAR_PTR test_str_to_cpy = (CHAR_PTR) "String to copy";
    char     target_string[STR_MAX_LEN];
    Library::strcpy(test_str_to_cpy, target_string);

    str_cmp_result = Library::strcmp(test_str_to_cpy, target_string);
    ASSERT_FATAL(str_cmp_result == 0, "strcpy error 1")

    // strcat
    char     test_str_cat_target[STR_MAX_LEN] = "String 1 + ";
    CHAR_PTR test_str_to_cat                  = (CHAR_PTR) "String 2";
    Library::strcat(test_str_cat_target, test_str_to_cat);

    str_cmp_result = Library::strcmp("String 1 + String 2", test_str_cat_target);
    ASSERT_FATAL(str_cmp_result == 0, "strcat error 1")
}

void test_list() {
    /*
    Library::List<int8_t, 20> list;

    ASSERT_FATAL(list.length() == 0, "list error 1")
    ASSERT_FATAL(list.max_size() == 20, "list error 2")

    list.add('n');

    ASSERT_FATAL(list.length() == 1, "list error 3")
    ASSERT_FATAL(list[0] == 'n', "list error 4")

    list.add('O');
    list.add('S');

    ASSERT_FATAL(list.length() == 3, "list error 5")
    ASSERT_FATAL(list[1] == 'O', "list error 6")

    list.pop();
    ASSERT_FATAL(list.length() == 2, "list error 7")
    ASSERT_FATAL(list[1] == 'O', "list error 8")

    list[1] = 'B';
    ASSERT_FATAL(list[1] == 'B', "list error 9")

    list.clear();
    ASSERT_FATAL(list.length() == 0, "list error 10")
    ASSERT_FATAL(list.max_size() == 20, "list error 11")

    for (uint8_t i = 0; i < 25; i++)
        list.add(i);

    ASSERT_FATAL(list.length() == 20, "list error 12")
    ASSERT_FATAL(list[0] == 0, "list error 13")
    ASSERT_FATAL(list[19] == 19, "list error 14")

    list.clear();

    ASSERT_FATAL(list.length() == 0, "list error 15")
    ASSERT_FATAL(list.max_size() == 20, "list error 16")

    list.add('A');
    list.add('B');
    list.add('C');

    list.remove_at(1);

    ASSERT_FATAL(list.length() == 2, "list error 17")
    ASSERT_FATAL(list[1] == 'C', "list error 18")

    list.add('A');
    list.add('B');
    list.add('C');
    list.add('D');
    list.add('E');

    list.remove_at(0);
    list.remove_at(4);
    list.remove_at(list.length() - 1);

    ASSERT_FATAL(list.length() == 4, "list error 19")
    ASSERT_FATAL(list[0] == 'C', "list error 20")
    ASSERT_FATAL(list[1] == 'A', "list error 21")
    ASSERT_FATAL(list[2] == 'B', "list error 22")
    ASSERT_FATAL(list[3] == 'C', "list error 23")
    */
}

void test_array_utils() {
    uint8_t test_array_len = 20;
    int8_t  test_array[test_array_len];
    uint8_t test_array_pos = 0;

    for (uint8_t i = 0; i < 10; i++) {
        test_array[i]  = i;
        test_array_pos = i;
    }

    test_array_pos = Library::remove_at<int8_t>(test_array, test_array_len, test_array_pos, 5);
    ASSERT_FATAL(test_array[5] == 6, "arrayutils error 1")

    test_array_pos = Library::add_at<int8_t>(test_array, test_array_len, test_array_pos, 1, 69);
    ASSERT_FATAL(test_array[1] == 69, "arrayutils error 2")
    ASSERT_FATAL(test_array[2] == 1, "arrayutils error 3")
}
} // namespace Testing
