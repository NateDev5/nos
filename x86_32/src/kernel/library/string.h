#pragma once

#include <stdarg.h>
#include <utils/types.h>

// #define STR_MAX_LEN 512

namespace Library {
uint32_t strlen(IN CONST_CHAR_PTR in_str);
bool strcmp(IN CONST_CHAR_PTR in_str1, IN CONST_CHAR_PTR in_str2);
void strcpy(IN CHAR_PTR in_str, OUT CHAR_PTR out_str);
void strcat(IN CHAR_PTR in_str, IN_OUT CHAR_PTR out_str);
} // namespace Library
