
#include <iostream>

import syntropy.types;
import syntropy.language;
import syntropy.memory;
import syntropy.allocator;
import syntropy.buffer;
import syntropy.hash;
import syntropy.span;
import syntropy.unique;

struct Foo
{
    Foo()
    {
        _foo = 42;
    }

    sy::Int _foo{ 0 };
};

int main()
{
    {
        auto ag = sy::AllocatorGuard{ sy::GetSystemAllocator() };

        auto uf = sy::MakeUnique<Foo>();

        uf->_foo = 43;

        auto uf2 = sy::MakeUnique<Foo>();

        uf = sy::Move(uf2);
    }

    return 0;
}
