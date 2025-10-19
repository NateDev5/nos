#pragma once

#include <utils/types.h>

#include <kernel/library/log.h>

#include <stdarg.h>

#ifdef __DEBUG__
#define DEBUG_ERR(error) Debug::fprintln("(ERROR) [%s:%s]: %s", __FILE__, __func__, error);

#define DEBUG_RAW(msg)                                                                                                                                         \
    Debug::print_str(msg);                                                                                                                                     \
    Debug::print_char('\n');

#define DEBUG_PRINT(msg, ...) Debug::fprintln(msg, ##__VA_ARGS__);

#define TODO(todo)            Debug::fprintln("TODO in (%s:%s): %s", __FILE__, __func__, todo);

#define TEMP                  Debug::fprintln("TEMP function: %s:%s", __FILE__, __func__);

#define UNSAFE(msg)           Debug::fprintln("UNSAFE in (%s:%s): %s", __FILE__, __func__, msg);

#define DEBUG_PRINT_UNSAFE(msg, ...)                                                                                                                           \
    UNSAFE("Kernel debug print used")                                                                                                                          \
    DEBUG_PRINT(msg, ##__VA_ARGS__);

#define DEBUG_RAW_UNSAFE(msg) DEBUG_RAW(msg)

namespace Debug {
void print_char(IN char _char);
void print_str(IN CONST_CHAR_PTR _str);
void vprintf(IN CONST_CHAR_PTR format, IN va_list args);
void printf(IN CONST_CHAR_PTR format, ...);
void fprintln(IN CONST_CHAR_PTR format, ...);
} // namespace Debug
#else
#define DEBUG_ERR(...)
#define DEBUG_PRINT(...)
#define DEBUG_RAW(...)
#define TODO(...)
#define TEMP
#define UNSAFE(...)
#define DEBUG_PRINT_UNSAFE(...)
#define DEBUG_RAW_UNSAFE(...)
#endif
