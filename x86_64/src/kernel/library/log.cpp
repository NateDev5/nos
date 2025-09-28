#include <kernel/library/assert.h>
#include <kernel/library/log.h>
#include <kernel/library/panic.h>
#include <kernel/library/string.h>

#include <kernel/drivers/video/framebuffer.h>

#include <kernel/terminal/term.h>

#include <utils/math.h>

namespace Library {
void vprintf(IN CONST_CHAR_PTR format, IN va_list args) {
    ASSERT(format != NULL, "format is null")

    uint32_t format_len = strlen(format);
    for (uint32_t pos = 0; pos < format_len; pos++) {
        if (format[pos] == '%' && (pos + 1) < format_len) {
            pos++;

            switch (format[pos]) {
            case 's': {
                CHAR_PTR string = va_arg(args, CHAR_PTR);
                print(string);
                continue;
            }
            case 'c': {
                int64_t _char = va_arg(args, int64_t);
                printc(_char);
                continue;
            }
            case 'i': {
                int64_t value = va_arg(args, int64_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 10);
                print(temp_str);
                continue;
            }
            case 'b': {
                int64_t value = va_arg(args, int64_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 2);
                print(temp_str);
                continue;
            }
            case 'h': {
                int64_t value = va_arg(args, int64_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 16);
                print(temp_str);
                continue;
            }
            case 'H': {
                int64_t value = va_arg(args, int64_t);
                char    temp_str[STR_MAX_LEN];
                Math::itoa(value, temp_str, 16, true, true);
                print(temp_str);
                continue;
            }
            }
        }
        printc(format[pos]);
    }
}

void printf(IN CONST_CHAR_PTR format, IN...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void printf_ln(IN CONST_CHAR_PTR format, IN...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printc('\n');
}

void print(IN CONST_CHAR_PTR message) { printf(message); }

void println(IN CONST_CHAR_PTR message) { printf_ln(message); }

void printc(IN char _char) { Kernel::Terminal::print_char(_char); }
} // namespace Library
