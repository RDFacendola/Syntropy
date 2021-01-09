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

// ==================================================================================

std::ostream& operator<<(std::ostream& ss, Syntropy::Memory::Bytes rhs)
{
    return (ss << ToInt(rhs));
}

std::ostream& operator<<(std::ostream& ss, Syntropy::Memory::Byte rhs)
{
    return (ss << Syntropy::ToInt(rhs));
}

int main(int argc, char** argv)
{
    std::cout << "Hello Syntropy!\n";

    using namespace Syntropy::Memory::Literals;

    auto dbga = Syntropy::Memory::PolymorphicAllocator<DebugAllocator>();

    Syntropy::Memory::SetAllocator(dbga);

    Syntropy::Int array[] = { 0, 1, 2, 3, 4, 5 };

    auto span = Syntropy::Ranges::MakeSpan(Syntropy::PtrOf(array[0]), 6);

    auto bspan = Syntropy::Memory::RangeBytesOf(span);


    for (auto&& element : Reverse(span))
    {
        std::cout << element << "\n";
    }

    for (auto&& element : span)
    {
        std::cout << element << "\n";
    }

    // #TODO ADL functions create circular dependencies among ranges definitions. Move them outside!

    //auto b = bspan == bspan;

    system("pause");

}
