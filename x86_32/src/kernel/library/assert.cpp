#include <kernel/library/assert.h>

#include <kernel/library/panic.h>

namespace Kernel {
    void assert(IN bool expression, IN const cstr msg)
    {
        if (!expression)
            Kernel::panic(msg);
    }
}