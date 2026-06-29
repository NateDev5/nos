#pragma once

#include <utils/types.h>

#include <kernel/library/debug.h>

#include <stdarg.h>

#define KLOG(msg, ...)                                                                                                                                         \
    DEBUG_LOG(msg, ##__VA_ARGS__)                                                                                                                                  \
    Library::printf_ln(msg, ##__VA_ARGS__);

#define KINFO(msg, ...)                                                                                                                                        \
    DEBUG_INFO(msg, ##__VA_ARGS__)                                                                                                                             \
    Library::printf("\033[0;34m[ INFO ]\033[0m ");                                                                                                             \
    Library::printf_ln(msg, ##__VA_ARGS__);

#define KWARN(msg, ...)                                                                                                                                        \
    DEBUG_WARN(msg, ##__VA_ARGS__)                                                                                                                             \
    Library::printf("\033[0;33m[ WARNING ]\033[0m ");                                                                                                          \
    Library::printf_ln(msg, ##__VA_ARGS__);

#define KERROR(msg, ...)                                                                                                                                       \
    DEBUG_ERROR(msg, ##__VA_ARGS__)                                                                                                                            \
    Library::printf("\033[0;31m[ ERROR ]\033[0m ");                                                                                                            \
    Library::printf_ln(msg, ##__VA_ARGS__);

#define KFATAL(msg, ...)                                                                                                                                       \
    DEBUG_FATAL(msg, ##__VA_ARGS__)                                                                                                                            \
    Library::printf("\033[0;31m[ FATAL ]\033[0m ");                                                                                                            \
    Library::printf_ln(msg, ##__VA_ARGS__);

#define KPANIC(msg, ...)                                                                                                                                       \
    DEBUG_FATAL(msg, ##__VA_ARGS__)                                                                                                                            \
    Library::printf("\033[0;31m[ PANIC ]\033[0m ");                                                                                                            \
    Library::printf_ln(msg, ##__VA_ARGS__);

namespace Library {
/*
%s for a string
%c for a char
%i for an int
%b for a binary representation of an int
%h for an hexadecimal representation of an int
*/
void vprintf(IN CONST_CHAR_PTR format, IN va_list args);

void printf(IN CONST_CHAR_PTR format, IN...);
void printf_ln(IN CONST_CHAR_PTR format, IN...);
void print(IN CONST_CHAR_PTR message);
void println(IN CONST_CHAR_PTR message);
void printc(IN char _char);
} // namespace Library
