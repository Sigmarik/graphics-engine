#include "murmur.h"

#include <string.h>

static hash_t cycle_left(hash_t num, unsigned short shift) {
    hash_t prefix = (num >> (sizeof(hash_t) * 8 - shift));

    return (num << shift) + prefix;
}

hash_t murmur_hash(const char* string) {
    hash_t value = 0xBAADF00DDEADBEEF;

    const char* end = string + strlen(string);

    for (const char* segment = string; segment < end; ++segment) {
        hash_t current = 0;
        for (unsigned char_id = 0; char_id < sizeof(hash_t) && segment < end;
             ++char_id, ++segment) {
            current <<= 8;
            current += (hash_t)*segment;
        }

        current = cycle_left(current * 0xDED15DED, 31) * 0xCADAB8A9;
        current ^= value;
        current = cycle_left(current, 15) * 0x112C13AB + 0x314159265358979;
        value = current;
    }

    return value;
}
