#pragma once

#include <utils/types.h>

#include <kernel/memory/mem.h>

#include <kernel/library/panic.h>

namespace Library {
    template<typename T, uint32 dataSize>
    class List
    {
        private:
            uint32 listSize = dataSize;
            uint32 listPos = 0;
            T data[dataSize];
        public:
            List() {
                Memory::memset(data, 0, listSize);
            }

            ~List() {}

            void add(IN T element) {
                if(listPos > listSize) return;
                data[listPos] = element;
                listPos++;
            }

            void pop () {
                if(listPos == 0) return;
                listPos--;
                data[listPos] = NULL;
            }
            
            T& head () {
                return data[listPos - 1];
            }

            void removeAt (IN uint32 index) {
                if(index > listPos - 1) Kernel::panic("(List::removeAt) List index out of bounds");

                for(uint32 i = index; i < listPos; i++) {
                    data[i] = data[i+1];
                    data[i+1] = NULL;
                }
                
                listPos--;
            }

            void clear() {
                Memory::memset(data, 0, listSize);
                listPos = 0;
            }

            uint32 length () {
                if(listPos > listSize) return listSize;

                for(uint32 size = 0; size < listSize; size++)
                    if(data[size] == NULL) return size;

                return listSize;
            }

            T& operator[] (IN uint32 index) {
                if(index > listPos - 1) Kernel::panic("List index out of bounds");
                return data[index];
            } 

            uint32 maxSize () {
                return listSize;
            }

            T* rawData () {
                return data;
            }
    };
}