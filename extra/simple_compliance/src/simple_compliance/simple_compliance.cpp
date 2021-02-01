/// \file main.cpp
/// A simple project used to test library compliance.
///
/// \author Raffaele D. Facendola - February 2021

// =========================================================================  //

#include <iostream>

#include "syntropy/language/foundation/types.h"

int main()
{
   std::cout << "Hello, Syntropy\n";

   Syntropy::Int i = 42;

   std::cout << i << "\n";

   return 0;
}

// =========================================================================  //
