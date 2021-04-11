/// \file main.cpp
/// A simple project used to test library compliance.
///
/// \author Raffaele D. Facendola - February 2021

// ========================================================================= //

#include <iostream>

#include "simple_compliance/bulk.h"

int main()
{
    std::cout << "Hello, Syntropy\n";

    auto r = Syntropy::UnitTest::TestRunner();

    r.Run();

    std::cout << "\ngg\n";

    return 0;
}

// ========================================================================= //
