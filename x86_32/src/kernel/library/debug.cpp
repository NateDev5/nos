#include <kernel/library/debug.h>
#include <kernel/library/string.h>

#include <kernel/drivers/serial/uart.h>

#include <utils/asm.h>
#include <utils/math.h>

namespace Debug {
void putchar(IN int8 _char) { outb(COM_PORT_1, _char); }

void putstr(IN cstr _str) {
    for (uint16 pos = 0; _str[pos] != '\0'; pos++)
        putchar(_str[pos]);
};

void vprintf(IN cstr format, IN va_list args) {
    uint16 format_len = Library::strlen(format);
    for (uint16 pos = 0; format[pos] != '\0'; pos++) {
        if (format[pos] == '%' && (pos + 1) < format_len) {
            pos++;

            // string
            if (format[pos] == 's') {
                str string = va_arg(args, str);
                putstr(string);
                continue;
            }

            // char
            if (format[pos] == 'c') {
                int32 _char = va_arg(args, int32);
                putchar(_char);
                continue;
            }

            // int
            if (format[pos] == 'i') {
                int32 value = va_arg(args, int32);
                int8 temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 10);
                putstr(temp_str);
                continue;
            }

            // binary
            if (format[pos] == 'b') {
                int32 value = va_arg(args, int32);
                int8 temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 2);
                putstr(temp_str);
                continue;
            }

            // hex
            if (format[pos] == 'h') {
                int32 value = va_arg(args, int32);
                int8 temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 16);
                putstr(temp_str);
                continue;
            }
        }

        putchar(format[pos]);
    }
}

void printf(IN cstr format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void fprintln(IN cstr format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    putchar('\n');
    va_end(args);
}
} // namespace Debug
