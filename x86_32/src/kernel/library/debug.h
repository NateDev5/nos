#pragma once

#include <utils/types.h>

#include <stdarg.h>

#ifdef __DEBUG__
#define DEBUG_ERR(error)                                                       \
    Debug::fprintln("(ERROR) [%s:%s]: %s", __FILE__, __func__, error);

#define DEBUG_PRINT(msg, ...) Debug::fprintln(msg, ##__VA_ARGS__);

namespace Debug {
void putchar(IN char _char);
void putstr(IN CONST_CHAR_PTR _str);
void vprintf(IN CONST_CHAR_PTR format, IN va_list args);
void printf(IN CONST_CHAR_PTR format, ...);
void fprintln(IN CONST_CHAR_PTR format, ...);
} // namespace Debug
#else
#define DEBUG_ERR(error)      ((void)0);
#define DEBUG_PRINT(msg, ...) ((void)0);
#endif
