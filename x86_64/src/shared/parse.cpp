#include <shared/assert.h>
#include <shared/log.h>
#include <shared/parse.h>
#include <shared/math.h>
#include <shared/string.h>

namespace Shared {
int64_t parse_int(IN CONST_CHAR_PTR str) {
    ASSERT_RETURN(str != NULLPTR, "Input string is null", 0);

    int64_t  result  = 0;
    uint64_t str_ptr = 0;
    
    bool negative = false;

    if(str[str_ptr] == '-') {
        negative = true;
        str_ptr++;
    }

    uint64_t str_len = strlen(str) - 1;

    while(str[str_ptr] != '\0') {
        ASSERT_RETURN(is_numeric(str[str_ptr]), "Non numeric character reached while parsing", 0)
        uint64_t exp = str_len - str_ptr;
        exp = (exp != 0) ? pow(10, exp) : 1;
        result += (str[str_ptr] - 48) * exp;
        str_ptr++;
    }

    // two complement
    if(negative)
        result = (~result) + 1;

    return result;
}

bool is_numeric(IN char _char) {
    return _char >= 48 && _char <= 57;
}
} // namespace Shared::Parse
