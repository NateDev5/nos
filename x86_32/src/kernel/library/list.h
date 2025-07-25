#pragma once

#include <utils/types.h>

#include <kernel/memory/mem.h>

#include <kernel/library/panic.h>

namespace Library {
    template<typename T, uint32 data_size>
    class List
    {
        private:
            uint32 lise_size = data_size;
            uint32 list_pos = 0;
            T data[data_size];
        public:
            List() {
                Memory::memset(data, 0, lise_size);
            }

            ~List() {}

            void add(IN T element) {
                if(list_pos > lise_size) return;
                data[list_pos] = element;
                list_pos++;
            }

            void pop () {
                if(list_pos == 0) return;
                list_pos--;
                data[list_pos] = NULL;
            }
            
            T& head () {
                return data[list_pos - 1];
            }

            void remove_at (IN uint32 index) {
                if(index > list_pos - 1) Kernel::panic("(List::removeAt) List index out of bounds");

                for(uint32 i = index; i < list_pos; i++) {
                    data[i] = data[i+1];
                    data[i+1] = NULL;
                }
                
                list_pos--;
            }

            void clear() {
                Memory::memset(data, 0, lise_size);
                list_pos = 0;
            }

            uint32 length () {
                if(list_pos > lise_size) return lise_size;

                for(uint32 size = 0; size < lise_size; size++)
                    if(data[size] == NULL) return size;

                return lise_size;
            }

            T& operator[] (IN uint32 index) {
                if(index > list_pos - 1) Kernel::panic("List index out of bounds");
                return data[index];
            } 

            uint32 max_size () {
                return lise_size;
            }

            T* raw_data () {
                return data;
            }
    };
}
