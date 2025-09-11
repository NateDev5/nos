#include "kernel/library/debug.h"
#include <cstdint>
#include <kernel/library/assert.h>
#include <kernel/library/panic.h>
#include <kernel/library/string.h>

#include <utils/math.h>

#include <kernel/memory/mem.h>

namespace Library {
uint32_t strlen(IN CONST_CHAR_PTR in_str) {
    ASSERT_RETURN(in_str != NULL, "input string is null", 0)

    for (uint32_t len = 0; len < STR_MAX_LEN; len++) {
        if (in_str[len] == '\0')
            return len;
    }

    DEBUG_ERR("unterminated string")
    return STR_MAX_LEN;
}

int32_t strcmp(IN CONST_CHAR_PTR in_str1, IN CONST_CHAR_PTR in_str2) {
    ASSERT_RETURN(in_str1 != NULL, "input string 1 is null", -1)
    ASSERT_RETURN(in_str2 != NULL, "input string 2 is null", -1)

    uint32_t pos = 0;

    while (pos < STR_MAX_LEN && in_str1[pos] != '\0' && in_str2[pos] != '\0' && in_str1[pos] == in_str2[pos])
        pos++;

    return (int32_t)in_str1[pos] - (int32_t)in_str2[pos];
}

void strcpy(IN CHAR_PTR in_str, OUT CHAR_PTR out_str) {
    ASSERT(in_str != NULL, "input string is null")
    ASSERT(out_str != NULL, "output string is null")

    for (uint32_t pos = 0; pos < STR_MAX_LEN; pos++) {
        out_str[pos] = in_str[pos];
        if (in_str[pos] == '\0')
            break;
    }
}

void strcat(IN_OUT CHAR_PTR target_str, IN CONST_CHAR_PTR to_cpy_str) {
    ASSERT(target_str != NULL, "target string is null")
    ASSERT(to_cpy_str != NULL, "string to copy is null")

    uint32_t target_str_len = strlen(target_str);

    for (uint32_t pos = 0; to_cpy_str[pos] != '\0'; pos++)
        target_str[target_str_len + pos] = to_cpy_str[pos];
}
} // namespace Library
