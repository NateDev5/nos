#include <kernel/library/string.h>
#include <kernel/library/panic.h>
#include <kernel/library/assert.h>

#include <utils/math.h>

#include <kernel/memory/mem.h>


namespace Library {
    uint16 strlen (IN cstr in_str) {
        if(in_str == NULL) Kernel::panic("(strlen) Input string is null");

        for(uint16 len = 0; len < STR_MAX_LEN; len++) 
            if(in_str[len] == '\0') return len;

        Kernel::panic("(strlen) String buffer overflow");
        return 0;
    }

    bool strcmp (IN cstr in_str1, IN cstr in_str2) {
        if(in_str1 == NULL) Kernel::panic("(strcmp) Input string 1 is null");
        if(in_str2 == NULL) Kernel::panic("(strcmp) Input string 2 is null");

        uint16 in_str1_len = strlen(in_str1);
        uint16 in_str2_len = strlen(in_str2);
        
        if(in_str1_len != in_str2_len) return false;

        for(uint16 pos = 0; pos < in_str1_len; pos++)
            if(in_str1[pos] != in_str2[pos]) return false;

        return true;
    }

    void strcpy (IN str in_str, OUT str out_str) {
        if(in_str == NULL) Kernel::panic("(strcpy) Input string is null");
        if(out_str == NULL) Kernel::panic("(strcpy) Output string is null");

        uint16 in_str_len = strlen(in_str);

        Memory::memset(out_str, 0, in_str_len);
        Memory::memcpy(in_str, out_str, in_str_len);
    }

    void strcat (IN str in_str, IN_OUT str out_str) {
        // if output string is too small it will corrupt memory
        if(in_str == NULL) Kernel::panic("(strcat) Input string is null");
        if(out_str == NULL) Kernel::panic("(strcat) Output string is null");

        uint16 in_str_len = strlen(in_str);
        uint16 out_str_len = strlen(out_str);

        /*
        if(sizeof(out_str) < (in_str1_len + in_str2_len)) 
            Debug::assert(false, "(strcat) Output string is too small for operation");
        */

        uint16 remaning_str_len = STR_MAX_LEN - out_str_len;
        if(in_str_len > remaning_str_len) Kernel::panic("(strcat) Input string is too big to concatenate with output string");

        Memory::memcpy(in_str, &out_str[out_str_len], in_str_len);
    }
}
