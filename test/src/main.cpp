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

#include "syntropy/memory/byte.h"
#include "syntropy/memory/address.h"
#include "syntropy/memory/size.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"
#include "syntropy/allocators/allocator.h"
#include "syntropy/allocators/system_allocator.h"

#include "syntropy/diagnostics/debugger.h"
#include "syntropy/diagnostics/assert.h"

/************************************************************************/
/* ENTRY POINT                                                          */
/************************************************************************/

struct Foo
{
    Syntropy::Fix8 a_{ 0 };
    Syntropy::Fix8 b_{ 0 };
    Syntropy::Fix8 c_{ 0 };
    Syntropy::Fix8 d_{ 0 };
};

int main(int argc, char **argv)
{
    std::cout << "Hello Syntropy!\n";

    using namespace Syntropy::Memory::Literals;
    
    auto& a = Syntropy::Memory::GetAllocator();

    auto buff = a.Allocate(32_Bytes, 8_Alignment);

}


