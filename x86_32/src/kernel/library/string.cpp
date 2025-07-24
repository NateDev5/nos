#include <kernel/library/string.h>
#include <kernel/library/panic.h>
#include <kernel/library/assert.h>

#include <utils/math.h>

#include <kernel/memory/mem.h>


namespace Library {
    uint16 strlen (IN cstr inStr) {
        if(inStr == NULL) Kernel::panic("(strlen) Input string is null");

        for(uint16 len = 0; len < STR_MAX_LEN; len++) 
            if(inStr[len] == '\0') return len;

        Kernel::panic("(strlen) String buffer overflow");
        return 0;
    }

    bool strcmp (IN cstr inStr1, IN cstr inStr2) {
        if(inStr1 == NULL) Kernel::panic("(strcmp) Input string 1 is null");
        if(inStr2 == NULL) Kernel::panic("(strcmp) Input string 2 is null");

        uint16 inStr1_len = strlen(inStr1);
        uint16 inStr2_len = strlen(inStr2);
        
        if(inStr1_len != inStr2_len) return false;

        for(uint16 pos = 0; pos < inStr1_len; pos++)
            if(inStr1[pos] != inStr2[pos]) return false;

        return true;
    }

    void strcpy (IN str inStr, OUT str outStr) {
        if(inStr == NULL) Kernel::panic("(strcpy) Input string is null");
        if(outStr == NULL) Kernel::panic("(strcpy) Output string is null");

        uint16 inStr_len = strlen(inStr);

        Memory::memset(outStr, 0, inStr_len);
        Memory::memcpy(inStr, outStr, inStr_len);
    }

    void strcat (IN str inStr, IN_OUT str outStr) {
        // if output string is too small it will corrupt memory
        if(inStr == NULL) Kernel::panic("(strcat) Input string is null");
        if(outStr == NULL) Kernel::panic("(strcat) Output string is null");

        uint16 inStr_len = strlen(inStr);
        uint16 outStr_len = strlen(outStr);

        /*
        if(sizeof(outStr) < (inStr1_len + inStr2_len)) 
            Debug::assert(false, "(strcat) Output string is too small for operation");
        */

        uint16 remaningStrLen = STR_MAX_LEN - outStr_len;
        if(inStr_len > remaningStrLen) Kernel::panic("(strcat) Input string is too big to concatenate with output string");

        Memory::memcpy(inStr, &outStr[outStr_len], inStr_len);
    }
}