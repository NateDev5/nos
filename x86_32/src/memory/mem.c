#include "mem.h"

void memset (IN PTRMEM address, IN uint8 data, IN uint32 size) {
    if(address == NULL) return;

    for(uint32 ui = 0; ui < size; ui++)
        address[ui] = data;
}

void memcpy (IN PTRMEM baseAddress, IN PTRMEM targetAddress, IN uint32 size) {
    if(baseAddress == NULL) return;
    if(targetAddress == NULL) return;

    for(uint32 ui = 0; ui <= size; ui++)
        targetAddress[ui] = baseAddress[ui];
}