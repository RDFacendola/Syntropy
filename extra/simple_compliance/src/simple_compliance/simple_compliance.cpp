/// \file main.cpp
/// A simple project used to test library compliance.
///
/// \author Raffaele D. Facendola - February 2021

// =========================================================================  //

#include <iostream>

#include "simple_compliance/bulk.h"

void Megaprint(Syntropy::Immutable<Syntropy::Strings::StringView> sv)
{
    std::cout << "hey!";
}

int main()
{
   std::cout << "Hello, Syntropy\n";

   Syntropy::Strings::String s0 = u8"ciao℧";
   Syntropy::Strings::String s1 = u8"ciao";
   Syntropy::Strings::String s2 = u8"caio";

   std::cout << (s1 == s2) << "\n";
   std::cout << (s0 == u8"ciaoasd") << "\n";
   std::cout << (u8"ciao℧" == s0) << "\n";

   Megaprint(s0);

   return 0;
}

// =========================================================================  //
