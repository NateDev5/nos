#include <shared/math.h>
#include <shared/assert.h>

namespace Shared {
int64_t abs(IN int64_t value) { return value >= 0 ? value : -value; }
uint64_t pow (IN uint64_t value, IN uint64_t exp) {
    ASSERT_RETURN(exp != 0, "Exponent is 0", value)
    ASSERT_RETURN(value != 0, "Value is 0", value)

    uint64_t result = value;
    while(--exp != 0)
        result *= value;

    return result;
}
} // namespace Shared::Math
