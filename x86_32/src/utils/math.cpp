#include <utils/math.h>

namespace Math {
    // from https://wiki.osdev.org/Printing_To_Screen
    str itoa(IN uint32 value, OUT str outStr, IN uint8 base) {
        str rc;
        str ptr;
        str low;

        if ( base < 2 || base > 36 )
        {
            *outStr = '\0';
            return outStr;
        }
        rc = ptr = outStr;

        // Set '-' for negative decimals.
        if ( value < 0 && base == 10 )
        {
            *ptr++ = '-';
        }

        // Remember where the numbers start.
        low = ptr;

        // The actual conversion.
        do
        {
            // Modulo is negative for negative value. This trick makes abs() unnecessary.
            *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
            value /= base;
        } while ( value );

        // Terminating the string.
        *ptr-- = '\0';

        // Invert the numbers.
        while ( low < ptr )
        {
            char tmp = *low;
            *low++ = *ptr;
            *ptr-- = tmp;
        }

        return rc;
    }
}