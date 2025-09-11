#include <kernel/library/debug.h>
#include <kernel/library/string.h>

#include <kernel/drivers/serial/uart.h>

#include <utils/asm.h>
#include <utils/math.h>

namespace Debug {
void print_char(IN char _char) { outb(COM_PORT_1, _char); }

void print_str(IN CONST_CHAR_PTR _str) {
    for (uint32_t pos = 0; _str[pos] != '\0'; pos++)
        print_char(_str[pos]);
};

void vprintf(IN CONST_CHAR_PTR format, IN va_list args) {
    uint32_t format_len = Library::strlen(format);
    for (uint32_t pos = 0; format[pos] != '\0'; pos++) {
        if (format[pos] == '%' && (pos + 1) < format_len) {
            pos++;

            switch (format[pos]) {
            case 's': {
                CHAR_PTR string = va_arg(args, CHAR_PTR);
                print_str(string);
                continue;
            }
            case 'c': {
                int64_t _char = va_arg(args, int64_t);
                print_char(_char);
                continue;
            }
            case 'i': {
                int64_t value = va_arg(args, int64_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 10);
                print_str(temp_str);
                continue;
            }
            case 'b': {
                int64_t value = va_arg(args, int64_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 2);
                print_str(temp_str);
                continue;
            }
            case 'h': {
                int64_t value = va_arg(args, int64_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 16);
                print_str(temp_str);
                continue;
            }
            case 'H': {
                int64_t value = va_arg(args, int64_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 16, true, true);
                print_str(temp_str); continue;
            }
            }
        }

        print_char(format[pos]);
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
    print_char('\n');
    va_end(args);
}
} // namespace Debug
