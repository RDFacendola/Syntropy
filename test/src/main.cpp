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

    auto t0 = Syntropy::MakeTuple(2);
    auto t1 = Syntropy::MakeTuple(2, 3);
    auto t2 = Syntropy::MakeTuple(4, 5);

    auto tl = Syntropy::Templates::TypeList<Syntropy::Int, Syntropy::Float>{};

    Foo(t0);
    Foo(t1);
    Foo(t2);

    Syntropy::LockstepApply([](auto& lhs, auto& rhs) { lhs += rhs; }, t1, t2);

    // Foo(tl);

    system("pause");

    return 0;
}


