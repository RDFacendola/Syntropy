/// \file main.cpp
/// A simple project used to test library compliance.
///
/// \author Raffaele D. Facendola - February 2021

// =========================================================================  //

#include <iostream>

#include "simple_compliance/bulk.h"

int main()
{
    std::cout << "Hello, Syntropy\n";

    auto array = Syntropy::FixArray<int, 10>{ 10, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    auto array_view = Syntropy::Ranges::ViewOf(array);

    static_assert(Syntropy::Concepts::IsSame<decltype(array_view),
                                             Syntropy::RWSpan<int>>);

    for(auto&& e : array)
    {
        std::cout << e << std::endl;
    }

    std::cout << "\ngg\n";

    return 0;
}

// =========================================================================  //
