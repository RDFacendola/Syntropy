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

    Syntropy::Tuple<int, float> t{ 10, 20.0f };

    std::cout << "\ngg\n";

    return 0;
}

// =========================================================================  //
