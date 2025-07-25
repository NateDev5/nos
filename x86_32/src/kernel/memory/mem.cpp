#include <kernel/memory/mem.h>

namespace Memory {
    void memset (IN PTRMEM address, IN uint8 data, IN uint32 size) {
        if(address == NULL) return;

        for(uint32 ui = 0; ui < size; ui++)
            address[ui] = data;
    }

    void memcpy (IN PTRMEM base_address, IN PTRMEM target_address, IN uint32 size) {
        if(base_address == NULL) return;
        if(target_address == NULL) return;

        for(uint32 ui = 0; ui <= size; ui++)
            target_address[ui] = base_address[ui];
    }
}
