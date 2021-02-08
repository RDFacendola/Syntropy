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

   Syntropy::Strings::String s = u8"ciao";

   Syntropy::Int i = s.GetLength();

   std::cout << i << "\n";

   return 0;
}

// =========================================================================  //
