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

    auto k = Base{};

    Syntropy::Memory::SetAllocator(dbga);

    auto x0 = Syntropy::Memory::Bits{ 0 };
    auto x00 = Syntropy::Memory::Bits{ 1 };

    auto x1 = Syntropy::Memory::Bytes{ 2 };
    auto x10 = Syntropy::Memory::Bytes{ 1 };

    auto x2 = Syntropy::Memory::KiloBytes{ 3 };
    auto x20 = Syntropy::Memory::KiloBytes{ 1 };

    auto x3 = Syntropy::Memory::MegaBytes{ 4 };
    auto x30 = Syntropy::Memory::MegaBytes{ 1 };
    
    auto x4 = Syntropy::Memory::GigaBytes{ 5 };
    auto x40 = Syntropy::Memory::GigaBytes{ 1 };
    
    auto x5 = Syntropy::Memory::TeraBytes{ 6 };
    auto x50 = Syntropy::Memory::TeraBytes{ 1 };
    
    auto x6 = Syntropy::Memory::KibiBytes{ 7 };
    auto x60 = Syntropy::Memory::KibiBytes{ 1 };
    
    auto x7 = Syntropy::Memory::MebiBytes{ 8 };
    auto x70 = Syntropy::Memory::MebiBytes{ 1 };
    
    auto x8 = Syntropy::Memory::GibiBytes{ 9 };
    auto x80 = Syntropy::Memory::GibiBytes{ 1 };
    
    auto x9 = Syntropy::Memory::TebiBytes{ 10 };
    auto x90 = Syntropy::Memory::TebiBytes{ 1 };

    auto x010 = Syntropy::Memory::Size<Syntropy::Templates::Ratio<30, 23>>{ 10 };

    auto p = Syntropy::MakeRWUnique<Derived>();

    auto sp = std::make_unique<Derived>();
    auto ep = std::make_unique<Derived>();

    ep.reset();

    system("pause");

}


