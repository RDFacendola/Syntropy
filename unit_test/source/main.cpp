
#include <iostream>

import syntropy.types;
import syntropy.memory;
import syntropy.allocator;
import syntropy.buffer;
import syntropy.hash;
import syntropy.span;

int main()
{
    {
        auto ag = sy::AllocatorGuard{ sy::GetSystemAllocator() };

        auto ma = sy::GetMaxAlignment();

        auto buff = sy::Buffer(sy::Bytes{ 64 });

        auto s = buff.GetSize();

        auto buffview = sy::ByteSpan{ buff };

        auto rwbuffview = sy::RWByteSpan{ buff };
    }

    return 0;
}
