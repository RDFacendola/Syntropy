/// \file simple_dependency.cpp
/// A simple projec depending on a static library.
///
/// \author Raffaele D. Facendola - January 2021

// =========================================================================  //

#include <iostream>

#include "simple_lib/simple_lib.h"

int main()
{
   int64_t number;

   std::cout << "Hello, Syntropy\n";

   std::cout << "Insert a number: ";
   std::cin >> number;

   std::cout << "Number double: " << DoubleThat(number) << "\n";

   return 0;
}

// =========================================================================  //
