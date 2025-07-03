#include "mem.h"

void memset (uint32* address, int32 data, int32 size) {
    for(int32 i = 0; i <= size; i++) 
        address[i] = data;
}

void memcpy (uint32* baseAddress, uint32* targetAddress, int32 size) {
    for(int32 i = 0; i <= size; i++)
        targetAddress[i] = baseAddress[i];
}