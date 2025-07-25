#pragma once 

#include <utils/types.h>

#include <kernel/library/panic.h>

namespace Library {
    template<typename T>
    uint32 remove_at (IN_OUT T* array, IN uint32 arrayLen, IN uint32 arrayPos, IN uint32 index) {
        if(array == NULL) Kernel::panic("(arrayutils::remove_at) Array is null");
        if(index > arrayPos) Kernel::panic("(arrayutils::remove_at) Index out of bounds");

        if(index == arrayLen - 1) {
            array[index] = NULL;

            arrayPos--;
            return arrayPos;
        }

        for(uint32 i = index; i < arrayPos; i++) {
            array[i] = array[i+1];
            array[i+1] = NULL;
        }

        arrayPos--;

        return arrayPos;
    }
}
