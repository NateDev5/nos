#include <shared/algo.h>
#include <shared/assert.h>
#include <shared/log.h>
#include <shared/mem.h>
#include <shared/string.h>

namespace Shared {
uint64_t strlen(IN CONST_CHAR_PTR in_str) {
    ASSERT_RETURN_RAW(in_str != NULL, "input string is null", 0)
    
    
    for (uint64_t len = 0; len < STR_MAX_LEN; len++) {
        if (in_str[len] == '\0')
            return len;
    }

    // DEBUG("unterminated string")
    return STR_MAX_LEN;
}

int64_t strcmp(IN CONST_CHAR_PTR in_str1, IN CONST_CHAR_PTR in_str2) {
    ASSERT_RETURN_RAW(in_str1 != NULL, "input string 1 is null", -1)
    ASSERT_RETURN_RAW(in_str2 != NULL, "input string 2 is null", -1)

    uint64_t pos = 0;

    while (pos < STR_MAX_LEN && in_str1[pos] != '\0' && in_str2[pos] != '\0' && in_str1[pos] == in_str2[pos])
        pos++;

    return (int64_t)in_str1[pos] - (int64_t)in_str2[pos];
}

void strcpy(IN CONST_CHAR_PTR in_str, OUT CHAR_PTR out_str) {
    ASSERT_RAW(in_str != NULL, "input string is null")
    ASSERT_RAW(out_str != NULL, "output string is null")

    for (uint64_t pos = 0; pos < STR_MAX_LEN; pos++) {
        out_str[pos] = in_str[pos];
        if (in_str[pos] == '\0')
            break;
    }
}

void strcat(IN_OUT CHAR_PTR target_str, IN CONST_CHAR_PTR to_cpy_str) {
    ASSERT_RAW(target_str != NULL, "target string is null")
    ASSERT_RAW(to_cpy_str != NULL, "string to copy is null")

    uint64_t target_str_len = strlen(target_str);

    for (uint64_t pos = 0; to_cpy_str[pos] != '\0'; pos++)
        target_str[target_str_len + pos] = to_cpy_str[pos];
}
} // namespace Shared
