#pragma once

#include <kernel/library/debug.h>
#include <utils/types.h>

#define ASSERT(expression, msg)                                                \
    if (!(expression)) {                                                       \
        DEBUG_ERR(msg)                                                         \
        return;                                                                \
    }

#define ASSERT_RETURN(expression, msg, ret)                                    \
    if (!(expression)) {                                                       \
        DEBUG_ERR(msg)                                                         \
        return ret;                                                            \
    }

#define ASSERT_FATAL(expression, msg)                                          \
    if (!(expression)) {                                                       \
        DEBUG_ERR(msg);                                                        \
        Kernel::panic(msg);                                                    \
    }
