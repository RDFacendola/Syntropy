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

namespace Witchery
{
    struct DoBar
    {
    public:

        template <typename TType>
        auto operator()(TType&& foo) const noexcept -> decltype((*this)(foo, Syntropy::Templates::kPriority<1>))
        {
            return (*this)(foo, Syntropy::Templates::kPriority<1>);
        }

    private:

        template <typename TType>
        auto operator()(TType&& foo, Syntropy::Templates::Priority<1>) const noexcept -> decltype(foo.Bar())
        {
            return foo.Bar();
        }

        template <typename TType>
        auto operator()(TType&& foo, Syntropy::Templates::Priority<0>) const noexcept -> decltype(Bar(foo))
        {
            return Bar(foo);
        }

    };

    inline constexpr auto Bar = DoBar{};
}

namespace BlackMagic
{
    struct MyClass
    {
        void Bar()
        {
            std::cout << "member function, witches!\n";
        }
    };

    struct YourClass
    {
        void Bar()
        {
            std::cout << "MOAR specialized function, witches!\n";
        }
    };

    void Bar(const YourClass& yc)
    {
        std::cout << "ADL function, witches!\n";
    }
}


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

        auto mc = BlackMagic::MyClass{};
        auto yc = BlackMagic::YourClass{};

        Witchery::Bar(mc);
        Witchery::Bar(yc);

        system("pause");
    }

    system("pause");

}
