#include <kernel/library/string.h>

#include <kernel/debug/assert.h>

#include <kernel/memory/mem.h>

namespace Library {
    uint16 strlen (IN cstr inStr) {
        for(uint16 len = 0; len < STR_MAX_LEN; len++) 
            if(inStr[len] == '\0') return len;

        Debug::assert(false, "String buffer overflow");
        return 0;
    }

    bool strcmp (IN cstr inStr1, IN cstr inStr2) {
        uint16 inStr1_len = strlen(inStr1);
        uint16 inStr2_len = strlen(inStr2);
        
        if(inStr1_len != inStr2_len) return false;

        for(uint16 pos = 0; pos < inStr1_len; pos++)
            if(inStr1[pos] != inStr2[pos]) return false;

        return true;
    }

    void strcpy (IN str inStr, OUT str outStr) {
        uint16 inStr_len = strlen(inStr);

        Memory::memset(outStr, 0, inStr_len);
        Memory::memcpy(inStr, outStr, inStr_len);
    }
}