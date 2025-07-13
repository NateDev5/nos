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
    void vprintf(IN cstr format, IN va_list args);

    /*
    %s for a string
    %c for a char
    %i for an int
    %b for a binary representation of an int
    %h for an hexadecimal representation of an int
    */
    void printf(IN cstr format, IN ...);

    /*
    %s for a string
    %c for a char
    %i for an int
    %b for a binary representation of an int
    %h for an hexadecimal representation of an int
    */
    void println_f(IN cstr format, IN ...);
    void print(IN cstr message);
    void println (IN cstr message);
    void fprintln (IN cstr message, IN uint8 format);
    void printc (IN int8 _char);
    void clear ();
}