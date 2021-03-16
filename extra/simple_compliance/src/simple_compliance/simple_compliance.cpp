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

   auto fix_array = Syntropy::FixArray<int, 5>(0, 1, 2, 3, 4);

   auto view = Syntropy::Ranges::ViewOf(fix_array);

   return 0;
}

// =========================================================================  //
