#pragma once

#include <utils/types.h>
#include <kernel/library/stdarg.h>

#define STR_MAX_LEN         512

namespace Library {
    uint16 strlen (IN cstr in_str);
    bool strcmp (IN cstr in_str1, IN cstr in_str2);
    void strcpy (IN str in_str, OUT str out_str);
    void strcat (IN str in_str, IN_OUT str out_str);
}
