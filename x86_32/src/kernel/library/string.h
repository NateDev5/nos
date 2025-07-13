#pragma once

#include <utils/types.h>
#include <kernel/library/stdarg.h>

#define STR_MAX_LEN         512

namespace Library {
    uint16 strlen (IN cstr inStr);
    bool strcmp (IN cstr inStr1, IN cstr inStr2);
    void strcpy (IN str inStr, OUT str outStr);
    void strcat (IN str inStr, IN_OUT str outStr);
}