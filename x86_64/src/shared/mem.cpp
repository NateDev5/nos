#include <shared/assert.h>
#include <shared/mem.h>

namespace Shared {
void memset(IN PTR address, IN uint8_t data, IN uint64_t size) {
    ASSERT(address != NULL, "memory address is null")

    // for (uint64_t i = 0; i < size; i++)
    // address[i] = data;

    // uint64_t blocks = size / 64;
    // uint64_t remaning = size - (blocks * 64);

    uint8_t *ptr = (uint8_t *)address;
    for (uint64_t i = 0; i < size; i++) {
        ptr[i] = data;
        // DEBUG_PRINT("%H, %i", ptr + i, i)
    }
    // for(uint64_t i = 0; i < size; i++) {
    // ptr[i] = data;
    // }
}

void memcpy(IN PTR base_address, IN PTR target_address, IN uint64_t size, IN bool reversed) {
    ASSERT(base_address != NULL, "base memory address is null")
    ASSERT(target_address != NULL, "target memory address is null")

    uint8_t *base_address_ptr   = (uint8_t *)base_address;
    uint8_t *target_address_ptr = (uint8_t *)target_address;

    if (reversed) {
        for (uint64_t i = size; i != 0; i--)
            target_address_ptr[i] = base_address_ptr[i];
    } else {
        for (uint64_t i = 0; i < size; i++)
            target_address_ptr[i] = base_address_ptr[i];
    }
    /*
    if (reversed) {
        for (uint64_t i = size; i != 0; i--)
            target_address[i] = base_address[i];

        target_address[0] = base_address[0];
    } else {
        for (uint64_t i = 0; i <= size; i++)
            target_address[i] = base_address[i];
    }
*/
}
} // namespace Shared
