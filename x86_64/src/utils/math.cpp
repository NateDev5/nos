#include <utils/math.h>

#include <kernel/library/string.h>

namespace Math {
// from https://wiki.osdev.org/Printing_To_Screen
CHAR_PTR itoa(IN int64_t value, OUT CHAR_PTR outStr, IN uint8_t base, IN bool caps, IN bool include_zeros) {
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

    if (base == 16) {
        *ptr++ = '0';
        *ptr++ = 'x';
    }

    // Remember where the numbers start.
    low = ptr;

    CONST_CHAR_PTR buffer      = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";
    CONST_CHAR_PTR buffer_caps = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUBWXYZ";
    
    uint64_t num_of_chars = 0;

    // The actual conversion.
    do {
        // Modulo is negative for negative value. This trick makes abs()
        // unnecessary.
        *ptr++ = (caps ? buffer_caps : buffer)[35 + value % base];
        value /= base;
        num_of_chars++;
    } while (value);

    if (base == 16 && include_zeros) {
        for (uint32_t i = 0; i < (16 - num_of_chars); i++)
            *ptr++ = '0';
    }

    // Terminate the string
    *ptr-- = '\0';

    // Invert the numbers.
    while (low < ptr) {
        char tmp = *low;
        *low++   = *ptr;
        *ptr--   = tmp;
    }

    return rc;
}
} // namespace Math
