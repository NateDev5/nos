#pragma once

#include <utils/types.h>

#include <kernel/library/assert.h>
#include <kernel/library/debug.h>

#define ASSERT(expression, msg)             KASSERT_UNSAFE(expression, msg)

#define ASSERT_RETURN(expression, msg, ret) KASSERT_RETURN_UNSAFE(expression, msg, ret)

#define ASSERT_RAW(expression, msg)                                                                                                                            \
    if (!(expression)) {                                                                                                                                       \
        DEBUG_RAW_UNSAFE(msg)                                                                                                                                  \
        return;                                                                                                                                                \
    }

#define ASSERT_RETURN_RAW(expression, msg, ret)                                                                                                                \
    if (!(expression)) {                                                                                                                                       \
        DEBUG_RAW_UNSAFE(msg)                                                                                                                                  \
        return ret;                                                                                                                                            \
    }
