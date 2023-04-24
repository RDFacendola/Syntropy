/// \file main.cpp
/// A simple hello world project.
///
/// \author Raffaele D. Facendola - January 2021

// =========================================================================  //

#include <iostream>

#include "simple_cxx20/simple_cxx20.h"

int main()
{
   std::cout << "Hello, Syntropy\n";

   std::cout << "Foo is FooConcept: " << FooConcept<Foo> << "\n";
   std::cout << "Bar is FooConcept: " << FooConcept<Bar> << "\n";

   return 0;
}

// =========================================================================  //
