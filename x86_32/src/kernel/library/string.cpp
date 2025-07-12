#include <kernel/library/string.h>

#include <kernel/debug/assert.h>

#include <kernel/memory/mem.h>

#include <cstdarg>

namespace Library {
    uint16 strlen (IN cstr inStr) {
        if(inStr == NULL) Debug::assert(false, "(strlen) Input string is null");

        for(uint16 len = 0; len < STR_MAX_LEN; len++) 
            if(inStr[len] == '\0') return len;

        Debug::assert(false, "(strlen) String buffer overflow");
        return 0;
    }

    bool strcmp (IN cstr inStr1, IN cstr inStr2) {
        if(inStr1 == NULL) Debug::assert(false, "(strcmp) Input string 1 is null");
        if(inStr2 == NULL) Debug::assert(false, "(strcmp) Input string 2 is null");

        uint16 inStr1_len = strlen(inStr1);
        uint16 inStr2_len = strlen(inStr2);
        
        if(inStr1_len != inStr2_len) return false;

        for(uint16 pos = 0; pos < inStr1_len; pos++)
            if(inStr1[pos] != inStr2[pos]) return false;

        return true;
    }

    void strcpy (IN str inStr, OUT str outStr) {
        if(inStr == NULL) Debug::assert(false, "(strcpy) Input string is null");
        if(outStr == NULL) Debug::assert(false, "(strcpy) Output string is null");

        uint16 inStr_len = strlen(inStr);

        Memory::memset(outStr, 0, inStr_len);
        Memory::memcpy(inStr, outStr, inStr_len);
    }

    void strcat (IN str inStr1, IN str inStr2, OUT str outStr) {
        // if output string is too small it will corrupt memory
        if(inStr1 == NULL) Debug::assert(false, "(strcat) Input string 1 is null");
        if(inStr2 == NULL) Debug::assert(false, "(strcat) Input string 2 is null");
        if(outStr == NULL) Debug::assert(false, "(strcat) Output string is null");

        uint16 inStr1_len = strlen(inStr1);
        uint16 inStr2_len = strlen(inStr2);

        /*
        if(sizeof(outStr) < (inStr1_len + inStr2_len)) 
            Debug::assert(false, "(strcat) Output string is too small for operation");
        */

        uint16 remaningStrLen = STR_MAX_LEN - inStr1_len;
        if(inStr2_len > remaningStrLen) Debug::assert(false, "(strcat) Input string is too big to concatenate with output string");

        Memory::memset(outStr, 0, (inStr1_len + inStr2_len));
        Memory::memcpy(inStr1, outStr, inStr1_len);
        Memory::memcpy(inStr2, &outStr[inStr1_len], inStr2_len);
    }

    void strfmt (IN str inStr, OUT str outStr, ...) {
        if(inStr == NULL) Debug::assert(false, "(strfmt) Input string is null");
        if(outStr == NULL) Debug::assert(false, "(strfmt) Output string is null");

        va_list args;
    }
}