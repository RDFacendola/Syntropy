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

/************************************************************************/
/* ENTRY POINT                                                          */
/************************************************************************/



template <Syntropy::Concepts::TupleLike T>
void Foo(Syntropy::Immutable<T> foo)
{

}

int main(int argc, char **argv)
{
    std::cout << "Hello Syntropy!\n";

    Syntropy::Int array0[] = { 42, 1, 2, 3, 4, 5 };
    Syntropy::Int array1[] = { 40, 2, 4, 6, 8, 10 };

    auto s = Syntropy::MakeSpan(&array0[0], 6);
    auto t = Syntropy::MakeSpan(&array1[0], 4);

    auto zz = Syntropy::Zip(s, t);

    auto f = Syntropy::Front(zz);
    auto pf = Syntropy::PopFront(zz);
    auto e = Syntropy::IsEmpty(zz);
    auto c = Syntropy::Count(zz);

    system("pause");

    return 0;
}


