#include <kernel/library/assert.h>
#include <kernel/library/log.h>
#include <kernel/library/panic.h>
#include <kernel/library/string.h>

#include <utils/math.h>

namespace Library {
void vfprintf(IN CONST_CHAR_PTR format, IN uint8_t color, IN va_list args) {
    ASSERT(format != NULL, "format is null")

    uint32_t format_len = strlen(format);
    for (uint32_t pos = 0; pos < format_len; pos++) {
        if (format[pos] == '%' && (pos + 1) < format_len) {
            pos++;

            // string
            if (format[pos] == 's') {
                CHAR_PTR string = va_arg(args, CHAR_PTR);
                Drivers::VGA::f_put_str(string, color);
                continue;
            }

            // char
            if (format[pos] == 'c') {
                int32_t _char = va_arg(args, int32_t);
                Drivers::VGA::f_put_char((int8_t)_char, color);
                continue;
            }

            // int
            if (format[pos] == 'i') {
                int32_t value = va_arg(args, int32_t);
                char    temp_str[11]; // int32_max 4,...,...,... , 10 characters +
                                      // 1 null
                Math::itoa(value, temp_str, 10);
                Drivers::VGA::f_put_str(temp_str, color);
                continue;
            }

            // binary
            if (format[pos] == 'b') {
                int32_t value = va_arg(args, int32_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 2);
                Drivers::VGA::f_put_str(temp_str, color);
                continue;
            }

            // hex
            if (format[pos] == 'h') {
                int32_t value = va_arg(args, int32_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 16);
                Drivers::VGA::f_put_str(temp_str, color);
                continue;
            }
        }

        Drivers::VGA::f_put_char(format[pos], color);
    }
}

void fprintf(IN CONST_CHAR_PTR format, IN uint8_t color, IN...) {
    va_list args;
    va_start(args, color);
    vfprintf(format, color, args);
    va_end(args);
}

void printf(IN CONST_CHAR_PTR format, IN...) {
    va_list args;
    va_start(args, format);
    vfprintf(format, Drivers::VGA::BWHITE, args);
    va_end(args);
}

void fprintf_ln(IN CONST_CHAR_PTR format, IN uint8_t color, IN...) {
    va_list args;
    va_start(args, color);
    vfprintf(format, color, args);
    va_end(args);
    Drivers::VGA::put_char('\n');
}

void printf_ln(IN CONST_CHAR_PTR format, IN...) {
    va_list args;
    va_start(args, format);
    vfprintf(format, Drivers::VGA::BWHITE, args);
    va_end(args);
    Drivers::VGA::put_char('\n');
}

void fprint(IN CONST_CHAR_PTR message, IN uint8_t color) {
    ASSERT(message != NULL, "message is null")
    Drivers::VGA::f_put_str(message, color);
}

void print(IN CONST_CHAR_PTR message) { fprint(message, Drivers::VGA::BWHITE); }

void fprintln(IN CONST_CHAR_PTR message, IN uint8_t format) {
    ASSERT(message != NULL, "message is null")
    Drivers::VGA::f_put_str(message, format);
    Drivers::VGA::put_char('\n');
}

void println(IN CONST_CHAR_PTR message) { fprintln(message, Drivers::VGA::BWHITE); }

void fprintc(IN char _char, IN uint8_t color) { Drivers::VGA::f_put_char(_char, color); }

void printc(IN char _char) { fprintc(_char, Drivers::VGA::BWHITE); }

void clear() { Drivers::VGA::clear_screen(); }
} // namespace Library
