#pragma once

#include <kernel/library/debug.h>
#include <utils/types.h>

#include <kernel/library/panic.h>

namespace Library {
template <typename T>
uint32 remove_at(IN_OUT T *array, IN uint32 array_len, IN uint32 array_pos,
                 IN uint32 index) {
    if (array == NULL)
        Kernel::panic("(arrayutils::remove_at) Array is null");
    if (index > array_pos)
        Kernel::panic("(arrayutils::remove_at) Index out of bounds");

    if (index == array_len - 1) {
        array[index] = NULL;

        array_pos--;
        return array_pos;
    }

    for (uint32 i = index; i < array_pos; i++) {
        array[i] = array[i + 1];
        array[i + 1] = NULL;
    }

    array_pos--;

    return array_pos;
}

template <typename T>
uint32 add_at(IN_OUT T *array, IN uint32 array_len, IN uint32 array_pos,
              IN uint32 index, IN T to_add) {
    if (array == NULL)
        Kernel::panic("(arrayutils::add_at) Array is null");
    if(to_add == NULL)
        Kernel::panic("(arrayutils::add_at) to_add is null");

    if (index > array_pos)
        Kernel::panic("(arrayutils::add_at) Index out of bounds 1");

    if(array_pos + 1 >= array_len)
        Kernel::panic("(arrayutils::add_at) Index out of bounds 2");

    for (uint32 i = array_pos; i > index - 1; i--)
        array[i + 1] = array[i];

    array[index] = to_add;

    array_pos++;

    return array_pos;
}
} // namespace Library
