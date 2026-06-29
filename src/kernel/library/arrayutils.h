#pragma once

#include <kernel/library/assert.h>
#include <kernel/library/debug.h>
#include <utils/types.h>

#include <kernel/library/panic.h>

namespace Library {
template <typename T> uint32_t remove_at(IN_OUT T *array, IN uint32_t array_len, IN uint32_t array_pos, IN uint32_t index) {
    KASSERT_RETURN(array != NULL, "array is null", array_pos)
    KASSERT_RETURN(index < array_pos, "index out of bounds", array_pos)

    if (index == array_len - 1) {
        array[index] = NULL;

        array_pos--;
        return array_pos;
    }

    for (uint32_t i = index; i < array_pos; i++) {
        array[i]     = array[i + 1];
        array[i + 1] = NULL;
    }

    array_pos--;

    return array_pos;
}

template <typename T> uint32_t add_at(IN_OUT T *array, IN uint32_t array_len, IN uint32_t array_pos, IN uint32_t index, IN T to_add) {
    KASSERT_RETURN(array != NULL, "array is null", array_pos)
    KASSERT_RETURN(to_add != NULL, "to_add is null", array_pos)
    KASSERT_RETURN(index <= array_pos, "index out of bounds 1", array_pos)
    KASSERT_RETURN(array_pos + 1 <= array_len, "index out of bounds 2", array_pos)

    if (index != array_pos) {
        for (uint32_t i = array_pos; i > index - 1; i--)
            array[i + 1] = array[i];
    }

    array[index] = to_add;

    array_pos++;

    return array_pos;
}
} // namespace Library
