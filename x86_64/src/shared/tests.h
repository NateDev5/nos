#pragma once

#include <utils/types.h>

#include <shared/humanize.h>
#include <shared/math.h>
#include <shared/mem.h>
#include <shared/testing.h>
#include <shared/string.h>
#include <shared/parse.h>

namespace Shared::Tests {
static CONST_CHAR_PTR test_str1 = "Test1";
static CONST_CHAR_PTR test_str2 = "Test2";
static uint8_t test_arr1[2] = {67, 67};
static uint8_t test_arr2[2] = {67, 67};

TEST(string, strlen, {
    uint64_t len = Shared::strlen(test_str1);
    EXPECT_EQ(len, 5)
})

TEST(string, strcmp, {
    int8_t result1 = Shared::strcmp (test_str1, test_str2);
    int8_t result2 = Shared::strcmp (test_str1, test_str1);
    EXPECT_EQ2(result1, -1, result2, 0)
})

TEST(string, strcat, {
    char test_cat[11] = "Test1";
    Shared::strcat(test_cat, test_str2);
    int8_t equal = Shared::strcmp(test_cat, "Test1Test2");
    EXPECT_EQ(equal, 0)
})

TEST(string, strcpy, {
    char test_cpy[6] = "Test2";
    Shared::strcpy(test_str1, test_cpy);
    int8_t equal = Shared::strcmp(test_cpy, "Test1");
    EXPECT_EQ(equal, 0)
})

TEST(math, abs, EXPECT_EQ(Shared::abs(-50), 50))

TEST(humanize, bytes, {
    CONST_CHAR_PTR alias;
    uint64_t       value = 50000;

    value = Shared::Humanize::bytes(value, alias);
    int8_t equal = Shared::strcmp(alias, "KB");

    EXPECT_EQ2(value, 48, equal, 0)
})

TEST(memory, memset, {
    Shared::memset(test_arr1, 0, 2);
    EXPECT_EQ2(test_arr1[0], 0, test_arr1[1], 0);
})

TEST(memory, memcpy, {
    Shared::memcpy(test_arr1, test_arr2, 2);
    EXPECT_EQ2(test_arr2[0], 0, test_arr2[1], 0);
})

TEST(parse, is_numeric, {
    bool first_res = Shared::is_numeric('A');
    bool second_res = Shared::is_numeric('6');

    EXPECT_EQ2(first_res, false, second_res, true);
})

TEST_GROUP(shared_lib, {
    USE_TEST(string, strlen)
    USE_TEST(string, strcmp)
    USE_TEST(string, strcat)
    USE_TEST(string, strcpy)

    USE_TEST(math, abs)
    USE_TEST(humanize, bytes)

    USE_TEST(memory, memset)
    USE_TEST(memory, memcpy)

    USE_TEST(parse, is_numeric)
})

} // namespace Shared::Tests
