#include <kernel/library/log.h>
#include <kernel/library/panic.h>
#include <kernel/library/string.h>

#include <utils/math.h>

namespace Library {
void vfprintf(IN cstr format, IN uint8 color_format, IN va_list args) {
    if (format == NULL)
        Kernel::panic("(vfprintf) Format is null");

    uint16 format_len = strlen(format);
    for (uint16 pos = 0; pos < format_len; pos++) {
        if (format[pos] == '%' && (pos + 1) < format_len) {
            pos++;

            // string
            if (format[pos] == 's') {
                str string = va_arg(args, str);
                Drivers::VGA::fputstr(string, color_format);
                continue;
            }

            // char
            if (format[pos] == 'c') {
                int32 _char = va_arg(args, int32);
                Drivers::VGA::fputchar((int8)_char, color_format);
                continue;
            }

            // int
            if (format[pos] == 'i') {
                int32 value = va_arg(args, int32);
                int8 temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 10);
                Drivers::VGA::fputstr(temp_str, color_format);
                continue;
            }

            // binary
            if (format[pos] == 'b') {
                int32 value = va_arg(args, int32);
                int8 temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 2);
                Drivers::VGA::fputstr(temp_str, color_format);
                continue;
            }

            // hex
            if (format[pos] == 'h') {
                int32 value = va_arg(args, int32);
                int8 temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 16);
                Drivers::VGA::fputstr(temp_str, color_format);
                continue;
            }
        }

        Drivers::VGA::fputchar(format[pos], color_format);
    }
}

void fprintf(IN cstr format, IN uint8 color_format, IN...) {
    va_list args;
    va_start(args, color_format);
    vfprintf(format, color_format, args);
    va_end(args);
}

void printf(IN cstr format, IN...) {
    va_list args;
    va_start(args, format);
    vfprintf(format, Drivers::VGA::BWHITE, args);
    va_end(args);
}

void fprintf_ln(IN cstr format, IN uint8 color_format, IN...) {
    va_list args;
    va_start(args, color_format);
    vfprintf(format, color_format, args);
    va_end(args);
    Drivers::VGA::putchar('\n');
}

void printf_ln(IN cstr format, IN...) {
    va_list args;
    va_start(args, format);
    vfprintf(format, Drivers::VGA::BWHITE, args);
    va_end(args);
    Drivers::VGA::putchar('\n');
}

void fprint(IN cstr message, IN uint8 color_format) {
    if (message == NULL)
        Kernel::panic("(fprint) Message is null");
    Drivers::VGA::fputstr(message, color_format);
}

void print(IN cstr message) { fprint(message, Drivers::VGA::BWHITE); }

void fprintln(IN cstr message, IN uint8 format) {
    if (message == NULL)
        Kernel::panic("(fprintln) Message is null");
    Drivers::VGA::fputstr(message, format);
    Drivers::VGA::putchar('\n');
}

void println(IN cstr message) { fprintln(message, Drivers::VGA::BWHITE); }

void fprintc(IN int8 _char, IN uint8 color_format) {
    Drivers::VGA::fputchar(_char, color_format);
}

void printc(IN int8 _char) { fprintc(_char, Drivers::VGA::BWHITE); }

void clear() { Drivers::VGA::clear_screen(); }
} // namespace Library
