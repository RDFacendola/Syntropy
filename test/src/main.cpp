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
#include "syntropy/memory/data_size.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"

/************************************************************************/
/* ENTRY POINT                                                          */
/************************************************************************/

struct Foo
{
    Foo(int aa, float bb, int cc)
        : a(aa)
        , b(bb)
        , c(cc)
    {}

    int a;
    float b;
    int c;
};

int main(int argc, char **argv)
{
    std::cout << "Hello Syntropy!\n";

    using namespace Syntropy::Literals;

    auto foo = [](float, float, int) {};

    auto t = Syntropy::Tuples::MakeTuple('p', 'a', 'l', 'l', 'i', 'n', 'o');
    auto u = Syntropy::Tuples::MakeTuple('p', 'a', 'l', 'l', 'e', 't', 't', 'o');
    auto v = Syntropy::Tuples::MakeTuple('c', 'o', 'n', 'i', 'g', 'l', 'i');

    auto rt = Syntropy::Ranges::MakeSpan(&Get<6>(t), 7);
    auto ru = Syntropy::Ranges::MakeSpan(&Get<7>(u), 8);
    auto rv = Syntropy::Ranges::MakeSpan(&Get<6>(v), 7);

    std::cout << "\n";
    for (auto&& rte : rt) std::cout << rte << "|";

    std::cout << "\n";
    for (auto&& rue : ru) std::cout << rue << "|";

    std::cout << "\n";
    for (auto&& rve : rv) std::cout << rve << "|";

    
    std::cout << "\n";

    auto ztr = Syntropy::Ranges::MakeZipRange(rt);
    auto zur = Syntropy::Ranges::MakeZipRange(ru);
    auto zvr = Syntropy::Ranges::MakeZipRange(rv);

    auto zt = Syntropy::Ranges::Zip(rt);
    auto ztu = Syntropy::Ranges::Zip(zt, ru);
    auto ztuv = Syntropy::Ranges::Zip(ztu, rv);
    auto zztuv = Syntropy::Ranges::Zip(rt, ru, rv);
    auto rztuv = Syntropy::Ranges::Reverse(ztuv);

    static_assert(Syntropy::Concepts::SameAs<decltype(zztuv), decltype(zztuv)>, "Noope!");

    for (auto&& [a, b, c] : rztuv)
    {
        std::cout << a << "," << b << "," << c << "|";
    }

    std::cout << "\n";
    system("pause");

    return 0;
}


