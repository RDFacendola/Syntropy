/// \file main.cpp
/// A simple project used to test library compliance.
///
/// \author Raffaele D. Facendola - February 2021

// =========================================================================  //

#include <iostream>

#include "simple_compliance/bulk.h"

struct FooView
{
    Syntropy::Int GetFront() const noexcept
    {
        return 43;
    }

    FooView PopFront() const noexcept
    {
        return {};
    }

    Syntropy::Bool IsEmpty() const noexcept
    {
        return false;
    }
};

struct Foo
{

};

int main()
{
    std::cout << "Hello, Syntropy\n";

    auto foo = Foo{};

   return 0;
}

// =========================================================================  //
