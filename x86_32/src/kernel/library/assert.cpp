#include <kernel/library/assert.h>

#include <kernel/library/panic.h>

namespace Kernel {
void assert(IN bool expression, IN const CONST_CHAR_PTR msg) {
    if (!expression)
        Kernel::panic(msg);
}

void assert_debug(IN bool expression, IN const CONST_CHAR_PTR msg) {
#ifdef __DEBUG__
    if (!expression)
        Kernel::panic(msg);
#else
    return;
#endif
}
} // namespace Kernel
