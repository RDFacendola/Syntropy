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
#include "syntropy/language/templates/priority.h"

#include "syntropy/language/preprocessor/macro.h"

#include "syntropy/language/support/initializer_list.h"
#include "syntropy/language/support/ignore.h"
#include "syntropy/language/support/swap.h"

#include "syntropy/core/support/scope_guard.h"
#include "syntropy/core/support/visitor.h"

#include "syntropy/core/concepts/ntuple.h"

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


struct MuhRange
{
    int GetFront() const { return 2; }
    MuhRange PopFront() const { return *this; }
    int IsEmpty() const { return 0; }

    int GetCount() const { return 10; }

    void* GetData() const { return nullptr; }

//     int GetBack() const {};
//     int PopBack() const {};
};

int Back(MuhRange mr) { return 1; }
MuhRange PopBack(MuhRange mr) { return mr; }
void* At(MuhRange, int) { return nullptr; };
MuhRange Slice(MuhRange a, int, int) { return a;  };

int main(int argc, char** argv)
{
    std::cout << "Hello Syntropy!\n";

    using namespace Syntropy::Memory::Literals;

    {
        Syntropy::Int arr[] = { 1, 2, 3 };
        Syntropy::FixArray<Syntropy::Int, 3> arr2 = { 1,2,3 };

        auto span = Syntropy::Ranges::MakeSpan(arr);
        auto bspan = Syntropy::Memory::MakeByteSpan(arr);
        auto fspan = Syntropy::RangeOf(arr2);

        auto p = MuhRange();

        static_assert(Syntropy::Ranges::Concepts::ForwardRange<MuhRange>, "nope!");
        static_assert(Syntropy::Ranges::Concepts::SizedRange<MuhRange>, "nope!");
        static_assert(Syntropy::Ranges::Concepts::BidirectionalRange<MuhRange>, "nope!");
        static_assert(Syntropy::Ranges::Concepts::RandomAccessRange<MuhRange>, "nope!");
        static_assert(Syntropy::Ranges::Concepts::ContiguousRange<MuhRange>, "nope!");

        auto l0 = Syntropy::Ranges::Front(p);
        auto l1 = Syntropy::Ranges::PopFront(p);
        auto l2 = Syntropy::Ranges::IsEmpty(p);
        auto l3 = Syntropy::Ranges::Count(p);
        auto l4 = Syntropy::Ranges::Back(p);
        auto l5 = Syntropy::Ranges::PopBack(p);
        auto l6 = Syntropy::Ranges::At(p, 0);
        auto l7 = Syntropy::Ranges::Slice(p, 0, 1);
        auto l8 = Syntropy::Ranges::Data(p);


        system("pause");
    }

    system("pause");

}
