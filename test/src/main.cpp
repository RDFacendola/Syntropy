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

#include "syntropy/core/foundation/tuple.h"
#include "syntropy/core/foundation/range.h"
#include "syntropy/core/foundation/reverse_range.h"
#include "syntropy/core/foundation/zip_range.h"
#include "syntropy/core/foundation/span.h"

#include "syntropy/memory/byte.h"
#include "syntropy/memory/address.h"
#include "syntropy/memory/data_size.h"
#include "syntropy/memory/alignment.h"
// #include "syntropy/memory/byte_span.h"

/************************************************************************/
/* ENTRY POINT                                                          */
/************************************************************************/

int main(int argc, char **argv)
{
    std::cout << "Hello Syntropy!\n";

    using namespace Syntropy::Literals;

    Syntropy::Byte x[] = { Syntropy::ToByte(1) };
    Syntropy::Int y[] = { 1 };

    auto s = Syntropy::Span<Syntropy::Byte>(&x[0], 3);
    auto t = Syntropy::Span<Syntropy::Byte>(&x[0], 3);

    auto w = Syntropy::MakeSpan(&x[0], 3);
    auto ww = Syntropy::Span<Syntropy::Byte>(w);

    s = PopBack(s, 10);

    auto rs = Syntropy::Reverse(s);
    auto rt = Syntropy::Reverse(t);

    auto cs = Count(s);

    Syntropy::AreEquivalent(s, t);

    auto ae = Syntropy::AreEquivalent(s, rt);

    system("pause");

    return 0;
}


