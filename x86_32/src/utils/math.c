#include "math.h"
#include "../video/video.h"

// from https://wiki.osdev.org/Printing_To_Screen
cstr itoa(int32 value, cstr str, int32 base) {
    cstr rc;
    cstr ptr;
    cstr low;

    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    
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