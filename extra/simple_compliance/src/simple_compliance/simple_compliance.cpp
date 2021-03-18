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

    auto fix_array = Syntropy::FixArray<int, 5>(10, 42, 2, 3, 4);

    static_assert(Syntropy::Ranges::Concepts::ForwardRange<decltype(fix_array)>,
                 "Not a foward range, buddy!");

    static_assert(Syntropy::Ranges::Concepts::SizedRange<decltype(fix_array)>,
                   "Not a sized range, buddy!");

    static_assert(Syntropy::Ranges::Concepts::BidirectionalRange<decltype(fix_array)>,
                   "Not a bidirectional range, buddy!");

    static_assert(Syntropy::Ranges::Concepts::RandomAccessRange<decltype(fix_array)>,
                   "Not a random access range, buddy!");

    static_assert(Syntropy::Ranges::Concepts::ContiguousRange<decltype(fix_array)>,
                   "Not a contiguous range, buddy!");

   return 0;
}

// =========================================================================  //
