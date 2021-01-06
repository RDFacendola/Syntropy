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

struct Base
{
    virtual ~Base() = default;

    int x;

    Syntropy::Tuples::Tuple<int, float> t0 { 10, 20.0f };
    Syntropy::Tuples::Tuple<> t3 {};
    std::tuple<int, float> t1{ 10, 20.0f };
    std::tuple<> t2 {};

};

struct Derived : Base
{
    virtual ~Derived() = default;

    float y;

};

int main(int argc, char** argv)
{
    std::cout << "Hello Syntropy!\n";

    auto dbga = Syntropy::Memory::PolymorphicAllocator<DebugAllocator>();

    Syntropy::Memory::SetAllocator(dbga);

    auto t00 = Syntropy::Tuples::MakeTuple();
    auto t01 = Syntropy::Tuples::MakeTuple(1);
    auto t02 = Syntropy::Tuples::MakeTuple(1, 2);
    auto t03 = Syntropy::Tuples::MakeTuple(1, 2, 3);
    auto t04 = Syntropy::Tuples::MakeTuple(1, 2, 3, 4 );
    auto t05 = Syntropy::Tuples::MakeTuple(1, 2, 3, 4, 5 );
    auto t06 = Syntropy::Tuples::MakeTuple(1, 2, 3, 4, 5, 6 );
    auto t07 = Syntropy::Tuples::MakeTuple(1, 2, 3, 4, 5, 6, 7);
    auto t08 = Syntropy::Tuples::MakeTuple(1, 2, 3, 4, 5, 6, 7, 8);
    auto t09 = Syntropy::Tuples::MakeTuple(1, 2, 3, 4, 5, 6, 7, 8, 9);
    auto t10 = Syntropy::Tuples::MakeTuple(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    auto t11 = Syntropy::Tuples::MakeTuple(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20);

    auto bx = Syntropy::PtrOf(Get<0>(t11)) - 19;

    auto vec = std::vector<int>{ 1,2, 3 };
    auto e = std::vector<int>{};

    auto span0 = Syntropy::Ranges::RWSpan<int>(bx, 20);
    auto span1 = Syntropy::Ranges::Span<int>(bx, 20);
    auto span2 = Syntropy::Ranges::Span<int>(bx, Syntropy::ToInt(0));

    system("pause");

}


