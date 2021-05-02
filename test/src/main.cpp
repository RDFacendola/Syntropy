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
#include "syntropy/language/templates/concepts.h"

#include "syntropy/language/preprocessor/macro.h"

#include "syntropy/language/support/initializer_list.h"
#include "syntropy/language/support/swap.h"
#include "syntropy/language/support/scope_guard.h"

#include "syntropy/core/toolset/visitor.h"

#include "syntropy/core/concepts/ntuple.h"

#include "syntropy/core/concepts/forward_range.h"
#include "syntropy/core/concepts/sized_range.h"
#include "syntropy/core/concepts/bidirectional_range.h"
#include "syntropy/core/concepts/random_access_range.h"
#include "syntropy/core/concepts/contiguous_range.h"
#include "syntropy/core/concepts/reverse_range.h"
#include "syntropy/core/concepts/zip_range.h"

#include "syntropy/core/foundation/tuple.h"
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

#include "syntropy/diagnostics/foundation/debugger.h"
#include "syntropy/diagnostics/foundation/assert.h"

#include "syntropy/core/toolset/event.h"

#include "syntropy/core/containers/fix_array.h"

int main(int argc, char** argv)
{
    std::cout << "Hello Syntropy!\n";

    using namespace Syntropy::Memory::Literals;

    {
        Syntropy::Int arr[] = { 1, 2, 3 };
        Syntropy::FixArray<Syntropy::Int, 3> arr2 = { 1,2,3 };

        auto span1 = Syntropy::MakeSpan(arr);
        auto span2 = Syntropy::MakeSpan(arr);

        auto span = Syntropy::Ranges::Zip(span1, span2);

        using TSpan = Syntropy::Templates::UnqualifiedOf<decltype(span1)>;

        static_assert(Syntropy::Ranges::Templates::ForwardRange<TSpan>, "nope!");
        static_assert(Syntropy::Ranges::Templates::SizedRange<TSpan>, "nope!");
        static_assert(Syntropy::Ranges::Templates::BidirectionalRange<TSpan>, "nope!");
        static_assert(Syntropy::Ranges::Templates::RandomAccessRange<TSpan>, "nope!");
        static_assert(Syntropy::Ranges::Templates::ContiguousRange<TSpan>, "nope!");

        auto x0 = Syntropy::Ranges::Front(span);
        auto y0 = Syntropy::Ranges::PopFront(span);
        auto z0 = Syntropy::Ranges::IsEmpty(span);
        auto w0 = Syntropy::Ranges::Count(span);
        auto xx0 = Syntropy::Ranges::Back(span);
        auto yy0 = Syntropy::Ranges::PopBack(span);
        auto zz0 = Syntropy::Ranges::At(span, 0);
        auto ww0 = Syntropy::Ranges::Slice(span, 0, 2);

        //for (auto e : span)
        //{
        //    std::cout << Syntropy::Sequences::Get<0>(e) << ";" << Syntropy::Sequences::Get<1>(e) << "\n";
        //}

        system("pause");
    }

    system("pause");

}
