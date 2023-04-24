
#include <iostream>

import syntropy.types;
import syntropy.memory;
import syntropy.allocator;
import syntropy.buffer;
import syntropy.hash;
import syntropy.span;

void* operator new(std::size_t sz)
{
    return std::malloc(sz);
}

void* operator new(std::size_t sz, std::align_val_t al)
{
    return std::malloc(sz);
}

void operator delete(void* ptr) noexcept
{
    std::free(ptr);
}

void operator delete(void* ptr, std::size_t sz) noexcept
{
    std::free(ptr);
}

void operator delete  (void* ptr, std::size_t sz, std::align_val_t al) noexcept
{
    std::free(ptr);
}

struct Foo
{
    virtual ~Foo() = default;

    virtual void Nice() {};

    sy::Fix8 f;
};

struct alignas(64) Bar : Foo
{
    virtual ~Bar() = default;

    virtual void Nice() override {};

    sy::Fix8 b;
};

int main()
{
    auto w = __STDCPP_DEFAULT_NEW_ALIGNMENT__;

    auto f = new Foo{};
    auto b = new Bar{};
    auto pb = (Foo*) b;

    auto fa = alignof(Foo);
    auto fb = alignof(Bar);

    delete f;
    delete pb;

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
