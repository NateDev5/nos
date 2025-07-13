#include <kernel/library/log.h>
#include <kernel/library/panic.h>
#include <kernel/library/string.h>

#include <utils/math.h>

namespace Library {
    void vprintf(IN cstr format, IN va_list args) {
        uint16 formatLen = strlen(format);
        for(uint16 pos = 0; pos < formatLen; pos++) {
            if(format[pos] == '%' && (pos + 1) < formatLen) {
                pos++;

                // string
                if(format[pos] == 's') {
                    str arg = va_arg(args, str);
                    Drivers::VGA::putstr(arg);
                    continue;
                }

                // char
                if(format[pos] == 'c') {
                    int32 _char = va_arg(args, int32);
                    Drivers::VGA::putchar((int8)_char);
                    continue;
                }

                // int
                if(format[pos] == 'i') {
                    int32 value = va_arg(args, int32);
                    int8 tempStr[STR_MAX_LEN];
                    Math::itoa(value, tempStr, 10);
                    Drivers::VGA::putstr(tempStr);
                    continue;
                }

                // binary
                if(format[pos] == 'b') {
                    int32 value = va_arg(args, int32);
                    int8 tempStr[STR_MAX_LEN];
                    Math::itoa(value, tempStr, 2);
                    Drivers::VGA::putstr(tempStr);
                    continue;
                }

                // hex
                if(format[pos] == 'h') {
                    int32 value = va_arg(args, int32);
                    int8 tempStr[STR_MAX_LEN];
                    Math::itoa(value, tempStr, 16);
                    Drivers::VGA::putstr(tempStr);
                    continue;
                }
            }

            Drivers::VGA::putchar(format[pos]);
        }
    }
    
    void printf(IN cstr format, IN ...) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }

    void println_f(IN cstr format, IN ...) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        Drivers::VGA::putchar('\n');
    }

    void print(IN cstr message) {
        Drivers::VGA::putstr(message);
    }

    void println (IN cstr message) {
        Drivers::VGA::putstr(message);
        Drivers::VGA::putchar('\n');
    }

    void fprintln (IN cstr message, IN uint8 format) {
        Drivers::VGA::fputstr(message, format);
        Drivers::VGA::putchar('\n');
    }

    void printc (IN int8 _char) {
        Drivers::VGA::putchar(_char);
    }

    void clear () {
        Drivers::VGA::clearScreen();
    }
}