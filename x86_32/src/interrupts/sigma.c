#include "../utils/types.h"

typedef struct InterruptFrameT {
    uint32 ip;
    uint32 cs;
    uint32 flags;
    uint32 sp;
    uint32 ss;
} InterruptFrameT;

__attribute__((interrupt))
void test (InterruptFrameT* test) {
    
}