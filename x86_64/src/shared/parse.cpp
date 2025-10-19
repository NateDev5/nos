#include <shared/assert.h>
#include <shared/log.h>
#include <shared/parse.h>

namespace Shared {
int64_t parse_int(IN CONST_CHAR_PTR str) {
    ASSERT_RETURN(str != NULLPTR, "Input string is null", 0);

    int64_t  result  = 0;
    uint64_t str_ptr = 0;

    if (str[str_ptr] == '-')
        result = ((result | 1) << 63);

    DEBUG("%b", result);

    return 0;
}

bool is_numeric(IN char _char) {
    return _char >= 48 && _char <= 57;
}
} // namespace Shared::Parse
