#pragma once

#include <utils/types.h>

#include <kernel/drivers/video/vga.h>

#include <stdarg.h>

namespace Library {
/*
%s for a string
%c for a char
%i for an int
%b for a binary representation of an int
%h for an hexadecimal representation of an int
*/
void vfprintf(IN CONST_CHAR_PTR format, IN uint8_t color, IN va_list args);

void fprintf(IN CONST_CHAR_PTR format, IN uint8_t color, IN...);
void printf(IN CONST_CHAR_PTR format, IN...);

void fprintf_ln(IN CONST_CHAR_PTR format, IN uint8_t color, IN...);
void printf_ln(IN CONST_CHAR_PTR format, IN...);

void fprint(IN CONST_CHAR_PTR message, IN uint8_t color);
void print(IN CONST_CHAR_PTR message);

void fprintln(IN CONST_CHAR_PTR message, IN uint8_t color);
void println(IN CONST_CHAR_PTR message);

void fprintc(IN char _char, IN uint8_t color);
void printc(IN char _char);

void clear();
} // namespace Library
