#pragma once

#include <kernel/library/debug.h>
#include <kernel/library/log.h>
#include <kernel/library/panic.h>

#include <utils/types.h>

#define KASSERT(expression, msg)                                                                                                                               \
    if (!(expression)) {                                                                                                                                       \
        DEBUG_ERROR(msg)                                                                                                                                       \
        return;                                                                                                                                                \
    }

#define KASSERT_RETURN(expression, msg, ret)                                                                                                                   \
    if (!(expression)) {                                                                                                                                       \
        DEBUG_ERROR(msg)                                                                                                                                       \
        return ret;                                                                                                                                            \
    }

#define KASSERT_FATAL(expression, msg)                                                                                                                         \
    if (!(expression))                                                                                                                                         \
        Kernel::panic(msg);

#define KASSERT_RETURN_UNSAFE(expression, msg, ret) KASSERT_RETURN(expression, msg, ret)
// UNSAFE("This function uses a direct kernel function which is unsafe for shared/usermode libraries")
#define KASSERT_UNSAFE(expression, msg)             KASSERT(expression, msg)
// UNSAFE("This function uses a direct kernel function which is unsafe for shared/usermode libraries")
