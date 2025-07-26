#pragma once

#include <utils/types.h>

#include <stdarg.h>

#ifndef NO_DEBUG
namespace Debug {
void putchar(IN char _char);
void putstr(IN CONST_CHAR_PTR _str);
void vprintf(IN CONST_CHAR_PTR format, IN va_list args);
void printf(IN CONST_CHAR_PTR format, ...);
void fprintln(IN CONST_CHAR_PTR format, ...);
} // namespace Debug
#endif // NO_DEBUG
