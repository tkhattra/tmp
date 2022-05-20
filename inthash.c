#include <stdint.h>

// Thomas Wang's integer hash functions.
// See https://web.archive.org/web/20111228072518/http://www.cris.com/~Ttwang/tech/inthash.htm.

uint32_t
hash32(uint32_t key)
{
    key = ~key + (key << 15);   // key = (key << 15) - key - 1;
    key ^= (key >> 12);
    key += (key << 2);
    key ^= (key >> 4);
    key *= 2057;                // key = (key + (key << 3)) + (key << 11);
    key ^= (key >> 16);
    return key;
}

uint64_t
hash64(uint64_t key)
{
    key = ~key + (key << 21);               // key = (key << 21) - key - 1;
    key ^= (key >> 24);
    key = (key + (key << 3)) + (key << 8);  // key * 265
    key ^= (key >> 14);
    key = (key + (key << 2)) + (key << 4);  // key * 21
    key ^= (key >> 28);
    key += (key << 31);
    return key;
}

uint32_t
hash6432(uint64_t key)
{
    key = ~key + (key << 18);   // key = (key << 18) - key - 1;
    key ^= (key >> 31);
    key *= 21;                  // key = (key + (key << 2)) + (key << 4);
    key ^= (key >> 11);
    key += (key << 6);
    key ^= (key >> 22);
    return (uint32_t) key;
}
