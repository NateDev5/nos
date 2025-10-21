#pragma once

#include <utils/types.h>

#include <shared/log.h>

#ifdef __TESTING__
#define DEBUG_TEST(msg, ...) DEBUG_CUSTOM("TEST", "\033[38;2;114;181;114m", msg, ##__VA_ARGS__)

#define TEST_GROUP(name, tests)                                                                                                                                \
    void test_group_##name() {                                                                                                                                 \
        DEBUG_TEST("\033[0;34mTest group (%s) \033[m", #name)                                                                                                       \
        tests                                                                                                                                                  \
    }

#define EXPECT_EQ(value, equal)                    return value == equal;

#define EXPECT_EQ2(value1, equal1, value2, equal2) return (value1 == equal1) && (value2 == equal2);

#define TEST(group, name, body)                                                                                                                                \
    bool test_##group##_##name(void) { body }

#define USE_TEST(group, name)                                                                                                                                  \
    {                                                                                                                                                          \
        bool test_result_##group##_##name = test_##group##_##name();                                                                                           \
        DEBUG_TEST("%s[%s] %s:%s\033[0m", test_result_##group##_##name ? "\033[0;32m" : "\033[0;31m", test_result_##group##_##name ? "\u2713" : "\u2715", #group,  \
              #name)                                                                                                                                           \
    }
#else
#define TEST_GROUP(...)
#define EXPECT_EQ(...)
#define EXPECT_EQ2(...)
#define TEST(...)
#define USE_TEST(...)
#endif
