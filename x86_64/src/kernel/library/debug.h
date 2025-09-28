#pragma once

#include <utils/types.h>

#include <kernel/library/log.h>

#include <stdarg.h>

#ifdef __DEBUG__
#define DEBUG_ERR(error)      Debug::fprintln("(ERROR) [%s:%s]: %s", __FILE__, __func__, error);

#define DEBUG_PRINT(msg, ...) Debug::fprintln(msg, ##__VA_ARGS__); 

#define TODO(todo) Debug::fprintln("TODO in (%s:%s): %s", __FILE__, __func__, todo);

#define TEMP Debug::fprintln("TEMP function: %s:%s", __FILE__, __func__);

namespace Debug {
void print_char(IN char _char);
void print_str(IN CONST_CHAR_PTR _str);
void vprintf(IN CONST_CHAR_PTR format, IN va_list args);
void printf(IN CONST_CHAR_PTR format, ...);
void fprintln(IN CONST_CHAR_PTR format, ...);
} // namespace Debug
#else
#define DEBUG_ERR(error)      ((void)0);
#define DEBUG_PRINT(msg, ...) ((void)0);
#endif
