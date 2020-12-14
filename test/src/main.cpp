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

int main(int argc, char **argv)
{
    std::cout << "Hello Syntropy!\n";

    using namespace Syntropy::Literals;

    auto t = Syntropy::MakeTuple('p', 'a', 'l', 'l', 'i', 'n', 'o');
    auto u = Syntropy::MakeTuple('p', 'a', 'l', 'l', 'e', 't', 't', 'o');
    auto v = Syntropy::MakeTuple('c', 'o', 'n', 'i', 'g', 'l', 'i');

    auto r = Syntropy::MakeSpan(&Get<6>(t), 7);
    auto s = Syntropy::MakeSpan(&Get<7>(u), 8);
    auto rr = Syntropy::Reverse(r);
    auto rs = Syntropy::Reverse(s);

    auto zrs = Syntropy::Zip(r, s);

    static_assert(Syntropy::Templates::IsDefaultConstructible<decltype(r)>, "nein0");
    static_assert(Syntropy::Templates::IsDefaultConstructible<decltype(rr)>, "nein1");
    static_assert(Syntropy::Templates::IsDefaultConstructible<decltype(zrs)>, "nein2");

    std::cout << "\nforward: ";

    for (auto&& x : r)
    {
        std::cout << x << ",";
    }

    std::cout << "\nbackward: ";

    for (auto&& x : rr)
    {
        std::cout << x << ",";
    }

    auto&& f = Front(zrs);

//     for (auto&& x : zrs)
//     {
//         //std::cout << "(" << x << ":" << y << "), ";
//     }

    std::cout << "\n";

    auto [a,b] = Syntropy::Ranges::MemberwiseSwap(r, s);

//     for (auto&& x : r)
//     {
//         std::cout << x << ",";
//     }

     auto tt = (t <=> t);
     auto tu = (t <=> u);
     auto tv = (t <=> v);
     
     auto ut = (u <=> t);
     auto uu = (u <=> u);
     auto uv = (u <=> v);
 
     auto vt = (v <=> t);
     auto vu = (v <=> u);
     auto vv = (v <=> v);

    system("pause");

    return 0;
}


