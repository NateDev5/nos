#include <utils/math.h>

namespace Math {
// from https://wiki.osdev.org/Printing_To_Screen
CHAR_PTR itoa(IN uint32_t value, OUT CHAR_PTR outStr, IN uint8_t base) {
    CHAR_PTR rc;
    CHAR_PTR ptr;
    CHAR_PTR low;

    if (base < 2 || base > 36) {
        *outStr = '\0';
        return outStr;
    }
    rc = ptr = outStr;

    // Set '-' for negative decimals.
    if (value < 0 && base == 10) {
        *ptr++ = '-';
    }

    // Remember where the numbers start.
    low = ptr;

    // The actual conversion.
    do {
        // Modulo is negative for negative value. This trick makes abs()
        // unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnop"
                 "qrstuvwxyz"[35 + value % base];
        value /= base;
    } while (value);

    // Terminating the string.
    *ptr-- = '\0';

    // Invert the numbers.
    while (low < ptr) {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }

    return rc;
}
} // namespace Math
