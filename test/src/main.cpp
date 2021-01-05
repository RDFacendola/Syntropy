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

int main(int argc, char **argv)
{
    std::cout << "Hello Syntropy!\n";

    auto dbga = Syntropy::Memory::PolymorphicAllocator<DebugAllocator>();

    Syntropy::Memory::SetAllocator(dbga);

    {
        auto event = Syntropy::Event<int>{};

        auto listener = Syntropy::Listener{};

        listener += event.Subscribe([](int x)
        {
            std::cout << "received " << x << "\n";
        });

        {
            auto listener_copy_constructed = listener;

            auto listener_copy_assigned = Syntropy::Listener{};

            listener_copy_assigned = listener_copy_constructed;

            event.Notify(1000);
        }

        event.Notify(2000);
    }

    system("pause");

}


