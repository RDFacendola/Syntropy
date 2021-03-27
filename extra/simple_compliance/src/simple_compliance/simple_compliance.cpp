/// \file main.cpp
/// A simple project used to test library compliance.
///
/// \author Raffaele D. Facendola - February 2021

// =========================================================================  //

#include <iostream>

#include "simple_compliance/bulk.h"

namespace Foo
{
    template <typename T, typename U>
    struct Bar
    {

    };


}

namespace Syntropy::Records
{
    template <typename T, typename U>
    struct RankTrait<Foo::Bar<T,U>>
        : Syntropy::Templates::IntConstant<2> {};

    template <typename T, typename U>
    struct ElementTypeTrait<0, Foo::Bar<T, U>>
        : Syntropy::Templates::Alias<T> {};

    template <typename T, typename U>
    struct ElementTypeTrait<1, Foo::Bar<T, U>>
        : Syntropy::Templates::Alias<U> {};

}

int main()
{
    std::cout << "Hello, Syntropy\n";

    using TTuple = Foo::Bar<int, float>;

    TTuple b;

    static_assert(Syntropy::Records::Record<TTuple>, "nope!");
    static_assert(Syntropy::Records::RankOf<TTuple> == 2, "nope!");
    static_assert(Syntropy::Records::IsSameRank<TTuple, TTuple>, "nope!");

    using T = Syntropy::Records::ElementTypeListOf<TTuple>;
    using Q = Syntropy::Templates::TypeList<int, float>;

    std::cout << Syntropy::Records::ElementIndexOf<int, TTuple> << std::endl;
    std::cout << Syntropy::Records::ElementIndexOf<float, TTuple> << std::endl;
    //std::cout << Syntropy::Records::ElementIndexOf<double, TTuple> << std::endl;



    std::cout << "\ngg\n";

    return 0;
}

// =========================================================================  //
