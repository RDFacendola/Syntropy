#include "syntropy/math/hash.h"

// @rfacendola I don't really feel like compiling this C goodness for two little functions. :)
//#include "fnv/fnv.h"

#include <numeric>

namespace syntropy
{
    /************************************************************************/
    /* HASH                                                                 */
    /************************************************************************/

    uint64_t Hash::Hash64(const void* buffer, size_t length)
    {
        // Uses FNV1a 64-bit with recommended constants by Landon Curt Noll - http://www.isthe.com/chongo/

        auto begin = reinterpret_cast<const uint8_t*>(buffer);

        return std::accumulate(begin, begin + length, uint64_t(0xcbf29ce484222325U), [](uint64_t hash, uint8_t character)
        {
            hash ^= static_cast<uint64_t>(character);

            return (hash << 0) + (hash << 1) + (hash << 4) + (hash << 5) + (hash << 7) + (hash << 8) + (hash << 40);
        });
    }

    uint32_t Hash::Hash32(const void* buffer, size_t length)
    {
        // Uses FNV1a 32-bit with recommended constants by Landon Curt Noll - http://www.isthe.com/chongo/

        auto begin = reinterpret_cast<const uint8_t*>(buffer);

        return std::accumulate(begin, begin + length, uint32_t(0x811c9dc5U), [](uint32_t hash, uint8_t character)
        {
            hash ^= static_cast<uint32_t>(character);

            return (hash << 0) + (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
        });
    }
}
