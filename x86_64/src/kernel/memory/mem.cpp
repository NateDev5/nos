#include <kernel/memory/mem.h>

#include <kernel/library/assert.h>

namespace Memory {
void memset(IN PTRMEM address, IN uint8_t data, IN uint64_t size) {
    ASSERT(address != NULL, "memory address is null")

    for (uint64_t i = 0; i < size; i++)
        address[i] = data;
}

void memcpy(IN PTRMEM base_address, IN PTRMEM target_address, IN uint64_t size, IN bool reversed) {
    ASSERT(base_address != NULL, "base memory address is null")
    ASSERT(target_address != NULL, "target memory address is null")

    if (reversed) {
        for (uint64_t i = size; i != 0; i--)
            target_address[i] = base_address[i];

        target_address[0] = base_address[0];
    } else {
        for (uint64_t i = 0; i <= size; i++)
            target_address[i] = base_address[i];
    }
}
} // namespace Memory
