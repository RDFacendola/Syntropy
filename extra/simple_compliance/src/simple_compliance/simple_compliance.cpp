/// \file main.cpp
/// A simple project used to test library compliance.
///
/// \author Raffaele D. Facendola - February 2021

// =========================================================================  //

#include <iostream>

#include "simple_compliance/bulk.h"

struct Foo
{

};

struct Bar : public Foo
{
    Bar() = default;
};

void DoBar(Syntropy::Tuple<Bar&> t)
{

}

int main()
{
    std::cout << "Hello, Syntropy\n";

    Bar b;

    Syntropy::Tuple<Foo&> p{b};
    Syntropy::Tuple<Bar&> q{b};

    Syntropy::Tuple<Foo&> r(b);
    Syntropy::Tuple<Bar&> s(b);

    auto t = Syntropy::Tuple<Foo&>{b};
    auto u = Syntropy::Tuple<Bar&>{b};

    auto v = Syntropy::Tuple<Foo&>(b);
    auto z = Syntropy::Tuple<Bar&>(b);

    std::cout << "Nice\n";

    DoBar(b);

    std::cout << "\ngg\n";

    return 0;
}

// =========================================================================  //
