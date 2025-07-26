#pragma once

#include <utils/types.h>

#include <kernel/library/stdarg.h>

#ifndef NO_DEBUG
namespace Debug {
void putchar (IN int8 _char);
void putstr (IN cstr _str);
void vprintf(IN cstr format, IN va_list args);
void printf(IN cstr format, ...); 
void fprintln (IN cstr format, ...);
}
#endif // NO_DEBUG
