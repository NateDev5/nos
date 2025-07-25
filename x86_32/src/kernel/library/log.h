#pragma once

#include <utils/types.h>

#include <kernel/drivers/video/vga.h>

#include <kernel/library/stdarg.h>

namespace Library {
    /*
    %s for a string
    %c for a char
    %i for an int
    %b for a binary representation of an int
    %h for an hexadecimal representation of an int
    */
    void vfprintf(IN cstr format, IN uint8 color_format, IN va_list args);

    void fprintf(IN cstr format, IN uint8 color_format, IN ...);
    void printf(IN cstr format, IN ...);

    void fprintf_ln(IN cstr format, IN uint8 color_format, IN ...);
    void printf_ln(IN cstr format, IN ...);

    void fprint(IN cstr message, IN uint8 color_format);
    void print(IN cstr message);

    void fprintln (IN cstr message, IN uint8 color_format);
    void println (IN cstr message);

    void fprintc (IN int8 _char, IN uint8 color_format);
    void printc (IN int8 _char);

    void clear ();
}
