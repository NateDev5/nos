#include <shared/humanize.h>
#include <shared/assert.h>
#include <shared/log.h>

namespace Shared::Humanize {

static CONST_CHAR_PTR aliases[7] = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};

uint64_t bytes(IN uint64_t value, OUT CONST_CHAR_PTR& alias) {
    // ASSERT_RETURN(alias != NULLPTR, "Alias is null", value);
    // ASSERT_RETURN(value > 0, "value is 0", value);

    if (value < 0x400) {
        alias = aliases[0];
        return value;
    } else if (IN_RANGE(value, KB_BOUNDS, MB_BOUNDS)) {
        alias = aliases[1];
        return value >> 10;
    } else if (IN_RANGE(value, MB_BOUNDS, GB_BOUNDS)) {
        alias = aliases[2];
        return value >> 20;
    } else if (IN_RANGE(value, GB_BOUNDS, TB_BOUNDS)) {
        alias = aliases[3];
        return value >> 30;
    } else if (IN_RANGE(value, TB_BOUNDS, PB_BOUNDS)) {
        alias = aliases[4];
        return value >> 40;
    } else if (IN_RANGE(value, PB_BOUNDS, EB_BOUNDS)) {
        alias = aliases[5];
        return value >> 50;
    } else {
        alias = aliases[6];
        return value >> 60;
    }
    return value;
}
} // namespace Shared::Humanize
