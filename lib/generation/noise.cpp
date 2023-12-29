#include "noise.h"

#include <math.h>
#include <stdlib.h>

double rand_double() { return (double)rand() / (double)RAND_MAX; }

static constexpr unsigned get_power(unsigned long long number) {
    return number < (1ull << 8)
               ? 8
               : (number < (1ull << 16) ? 16
                                        : (number < (1ull << 32) ? 32 : 64));
}

uint64_t rand_uint64() {
    constexpr unsigned RAND_POWER = get_power(RAND_MAX);

    uint64_t answer = 0;

    for (unsigned shift = 0; shift < 64; shift += RAND_POWER) {
        answer ^= ((uint64_t)rand() << shift);
    }

    return answer;
}
