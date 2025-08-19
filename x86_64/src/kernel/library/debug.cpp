#include <kernel/library/debug.h>
#include <kernel/library/string.h>

#include <kernel/drivers/serial/uart.h>

#include <utils/asm.h>
#include <utils/math.h>

namespace Debug {
void putchar(IN char _char) { outb(COM_PORT_1, _char); }

void putstr(IN CONST_CHAR_PTR _str) {
    for (uint32_t pos = 0; _str[pos] != '\0'; pos++)
        putchar(_str[pos]);
};

void vprintf(IN CONST_CHAR_PTR format, IN va_list args) {
    uint32_t format_len = Library::strlen(format);
    for (uint32_t pos = 0; format[pos] != '\0'; pos++) {
        if (format[pos] == '%' && (pos + 1) < format_len) {
            pos++;

            // string
            if (format[pos] == 's') {
                CHAR_PTR string = va_arg(args, CHAR_PTR);
                putstr(string);
                continue;
            }

            // char
            if (format[pos] == 'c') {
                int64_t _char = va_arg(args, int64_t);
                putchar(_char);
                continue;
            }

            // int
            if (format[pos] == 'i') {
                int64_t value = va_arg(args, int64_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 10);
                putstr(temp_str);
                continue;
            }

            // binary
            if (format[pos] == 'b') {
                int64_t value = va_arg(args, int64_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 2);
                putstr(temp_str);
                continue;
            }

            // hex
            if (format[pos] == 'h') {
                int64_t value = va_arg(args, int64_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 16);
                putstr(temp_str);
                continue;
            }

            // hex caps
            if (format[pos] == 'H') {
                int64_t value = va_arg(args, int64_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 16, true, true);
                putstr(temp_str);
                continue;
            }
        }

        putchar(format[pos]);
    }
}

void printf(IN CONST_CHAR_PTR format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void fprintln(IN CONST_CHAR_PTR format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    putchar('\n');
    va_end(args);
}
} // namespace Debug
