#pragma once

#include <utils/types.h>

#include <kernel/library/debug.h>

// change that to not call directly the kernel debug
#define DEBUG(msg, ...) DEBUG_DEBUG(msg, ##__VA_ARGS__)

namespace Shared::Log {}
