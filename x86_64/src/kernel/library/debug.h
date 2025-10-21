#pragma once

#include <utils/types.h>

#include <kernel/library/log.h>

#include <stdarg.h>

#ifdef __DEBUG__

#define DEBUG_INFO(msg, ...)                                                                                                                                   \
    Debug::printf("\033[0;34m[ INFO ]\033[0m ");                                                                                                               \
    Debug::fprintln(msg, ##__VA_ARGS__);

#define DEBUG_DEBUG(msg, ...)                                                                                                                                  \
    Debug::printf("\033[0;32m[ DEBUG ]\033[0m ");                                                                                                              \
    Debug::fprintln(msg, ##__VA_ARGS__);

#define DEBUG_WARN(msg, ...)                                                                                                                                   \
    Debug::printf("\033[0;33m[ WARNING ]\033[0m ");                                                                                                            \
    Debug::fprintln(msg, ##__VA_ARGS__);

#define DEBUG_ERROR(msg, ...)                                                                                                                                  \
    Debug::printf("\033[0;31m[ ERROR ] (%s:%s)\033[0m ", __FILE__, __func__);                                                                                  \
    Debug::fprintln(msg, ##__VA_ARGS__);

#define DEBUG_FATAL(msg, ...)                                                                                                                                  \
    Debug::printf("\033[0;91m[ FATAL ] (%s:%s)\033[0m ", __FILE__, __func__);                                                                                  \
    Debug::fprintln(msg, ##__VA_ARGS__);

#define DEBUG_CUSTOM(context, ansi_color, msg, ...)                                                                                                            \
    Debug::printf("%s[ %s ]\033[0m ", ansi_color, context);                                                                                                    \
    Debug::fprintln(msg, ##__VA_ARGS__);

#define DEBUG_LOG(msg, ...) Debug::fprintln(msg, ##__VA_ARGS__);

#define DEBUG_RAW(msg)                                                                                                                                         \
    Debug::print_str(msg);                                                                                                                                     \
    Debug::print_char('\n');

#define DEBUG_INFO_RAW(msg) \
    Debug::print_str("\033[0;34m[ INFO ]\033[0m "); \
    DEBUG_RAW(msg)

#define TODO(todo)  Debug::fprintln("\033[0;36m[ TODO ] (%s:%s)\033[0m: %s", __FILE__, __func__, todo);

#define TEMP        Debug::fprintln("\033[0;36m[ TEMP ]\033[0m: %s:%s", __FILE__, __func__);

#define UNSAFE(msg) Debug::fprintln("\33[0;35m[ UNSAFE ] (%s:%s)\033[0m: %s", __FILE__, __func__, msg);

#define DEBUG_PRINT_UNSAFE(msg, ...)                                                                                                                           \
    UNSAFE("Kernel debug print used")                                                                                                                          \
    DEBUG_INFO(msg, ##__VA_ARGS__);

#define DEBUG_RAW_UNSAFE(msg) DEBUG_RAW(msg)

namespace Debug {
void print_char(IN char _char);
void print_str(IN CONST_CHAR_PTR _str);
void vprintf(IN CONST_CHAR_PTR format, IN va_list args);
void printf(IN CONST_CHAR_PTR format, ...);
void fprintln(IN CONST_CHAR_PTR format, ...);
} // namespace Debug
#else
#define DEBUG_INFO(...)
#define DEBUG_DEBUG(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#define DEBUG_FATAL(...)
#define DEBUG_CUSTOM(...)
#define DEBUG_LOG(...)
#define DEBUG_RAW(...)
#define DEBUG_INFO_RAW(...)
#define TODO(...)
#define TEMP
#define UNSAFE(...)
#define DEBUG_PRINT_UNSAFE(...)
#define DEBUG_RAW_UNSAFE(...)
#endif
