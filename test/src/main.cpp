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
    MuhRange() = default;
    MuhRange(int* ptr, int size) {}

    int* GetData() const { return nullptr; }
    int GetCount() const { return 0; }
};

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

        auto x = Syntropy::Ranges::Details::RouteFront(p);
        auto y = Syntropy::Ranges::Details::RoutePopFront(p);
        auto z = Syntropy::Ranges::Details::RouteIsEmpty(p);
        auto w = Syntropy::Ranges::Details::RouteCount(p);
        auto xx = Syntropy::Ranges::Details::RouteBack(p);
        auto yy = Syntropy::Ranges::Details::RoutePopBack(p);
        auto zz = Syntropy::Ranges::Details::RouteAt(p, 0);
        auto ww = Syntropy::Ranges::Details::RouteSlice(p, 0, 2);
        auto xxx = Syntropy::Ranges::Details::RouteData(p);

        auto x0 = Syntropy::Ranges::Front(p);
        auto y0 = Syntropy::Ranges::PopFront(p);
        auto z0 = Syntropy::Ranges::IsEmpty(p);
        auto w0 = Syntropy::Ranges::Count(p);
        auto xx0 = Syntropy::Ranges::Back(p);
        auto yy0 = Syntropy::Ranges::PopBack(p);
        auto zz0 = Syntropy::Ranges::At(p, 0);
        auto ww0 = Syntropy::Ranges::Slice(p, 0, 2);
        auto xxx0 = Syntropy::Ranges::Data(p);

        system("pause");
    }

    system("pause");

}
