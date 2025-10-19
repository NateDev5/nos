#pragma once

#include <utils/types.h>

#include <kernel/library/debug.h>

#include <stdarg.h>

#define KLOG(msg, ...)                                                                                                                                         \
    DEBUG_PRINT(msg, ##__VA_ARGS__)                                                                                                                            \
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
