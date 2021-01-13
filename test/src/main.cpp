/// \file main.cpp
/// \brief Defines the entry point of the unit test application.
///
/// \author Raffaele D. Facendola - June 2020.

#include <iostream>
#include <string>

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/sequence.h"
#include "syntropy/language/templates/ratio.h"
#include "syntropy/language/templates/math.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/language/preprocessor/macro.h"

#include "syntropy/language/support/initializer_list.h"
#include "syntropy/language/support/ignore.h"
#include "syntropy/language/support/swap.h"

#include "syntropy/core/support/scope_guard.h"
#include "syntropy/core/support/visitor.h"

#include "syntropy/core/concepts/ntuple.h"

#include "syntropy/core/concepts/range.h"
#include "syntropy/core/concepts/forward_range.h"
#include "syntropy/core/concepts/sized_range.h"
#include "syntropy/core/concepts/bidirectional_range.h"
#include "syntropy/core/concepts/random_access_range.h"
#include "syntropy/core/concepts/contiguous_range.h"

#include "syntropy/core/foundation/tuple.h"
#include "syntropy/core/foundation/reverse_range.h"
#include "syntropy/core/foundation/zip_range.h"
#include "syntropy/core/foundation/span.h"
#include "syntropy/core/foundation/unique_ptr.h"

#include "syntropy/core/reflection/type_id.h"

#include "syntropy/memory/foundation/byte.h"
#include "syntropy/memory/foundation/address.h"
#include "syntropy/memory/foundation/size.h"
#include "syntropy/memory/foundation/alignment.h"
#include "syntropy/memory/foundation/byte_span.h"

#include "syntropy/memory/allocators/allocator.h"
#include "syntropy/memory/allocators/system_allocator.h"

#include "syntropy/diagnostics/debugger.h"
#include "syntropy/diagnostics/assert.h"

#include "syntropy/core/support/event.h"

#include "syntropy/core/containers/fix_array.h"

#include <tuple>
#include <vector>

class DebugAllocator : public Syntropy::Memory::SystemAllocator
{
public:


    Syntropy::Memory::RWByteSpan Allocate(Syntropy::Memory::Bytes size, Syntropy::Memory::Alignment alignment) noexcept
    {
        std::cout << "Allocating " << Syntropy::ToInt(size) << " bytes\n";

        allocated_ += size;

        return SystemAllocator::Allocate(size, alignment);
    }

    void Deallocate(Syntropy::Immutable<Syntropy::Memory::RWByteSpan> block, Syntropy::Memory::Alignment alignment) noexcept
    {
        SystemAllocator::Deallocate(block, alignment);

        deallocated_ += Count(block);

        std::cout << "Deallocating " << Syntropy::ToInt(Count(block)) << " bytes\n";
    }

    ~DebugAllocator()
    {
        SYNTROPY_ASSERT(allocated_ == deallocated_);        // Leak!
    }

    Syntropy::Memory::Bytes allocated_;
    Syntropy::Memory::Bytes deallocated_;

};

// ===========================================================================

struct Foo
{
    Foo() = delete;

    Foo(Syntropy::Int value) noexcept 
        : value_(value) {}

    Foo(Syntropy::Immutable<Foo> rhs) noexcept
    {
        value_ = rhs.value_;
    }

    Foo(Syntropy::Movable<Foo> rhs) noexcept
    {
        value_ = rhs.value_;
        rhs.value_ = -1;
    }

    Foo& operator=(Syntropy::Immutable<Foo> rhs) noexcept
    {
        value_ = rhs.value_;
        return *this;
    }

    Foo& operator=(Syntropy::Movable<Foo> rhs) noexcept
    {
        value_ = rhs.value_;
        rhs.value_ = -1;
        return *this;
    }

    Syntropy::Bool operator==(Syntropy::Immutable<Foo> rhs) const noexcept
    {
        return value_ == rhs.value_;
    }

    Syntropy::Ordering operator<=>(Syntropy::Immutable<Foo> rhs) const noexcept
    {
        return value_ <=> rhs.value_;
    }

    ~Foo()
    {
        std::cout << "mega-destructor\n";
    }

    Syntropy::Int value_;
};

struct Bar
{
    Bar(Syntropy::Int value) : value_(value) {}

    Syntropy::Int value_;
};

struct Baz
{
    Baz(Syntropy::Int value) : value_(value) {}

    Baz(Syntropy::Immutable<Bar> bar) : value_(bar.value_) {}

    Baz(Syntropy::Movable<Bar> bar)
        : value_(bar.value_)
    {
        bar.value_ = -1;
    }

    Syntropy::Int value_;
};

int main(int argc, char** argv)
{
    std::cout << "Hello Syntropy!\n";

    using namespace Syntropy::Memory::Literals;

    auto dbga = Syntropy::Memory::PolymorphicAllocator<DebugAllocator>();

    Syntropy::Memory::SetAllocator(dbga);

    {
        auto array = Syntropy::RWFixArray<Foo, 5>{ 10, 20, 30, 40, 50 };
        // auto erray = Syntropy::RWFixArray<Foo, 5>{};

        auto rwarray = Syntropy::RWFixArray<Foo, 5>{ 1, 2, 3, 4, 5 };
        auto rdarray = Syntropy::FixArray<Foo, 5>{ 1, 2, 3, 4, 5 };
        auto rgarray = Syntropy::FixArray<Foo, 5>{ 1, 2, 4, 4, 5 };

        auto a0 = (rwarray == rdarray);
        auto a1 = (rwarray == rgarray);
        auto a2 = (rwarray <=> rgarray);
        auto a3 = (rgarray <=> rwarray);

    }

    system("pause");

}
