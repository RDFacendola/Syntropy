/// \file tuple_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <tuple>

#include "syntropy/language/foundation.h"

#include "syntropy/experimental/core/foundation/tuple.h"

#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::Experimental::UnitTest
{
    /************************************************************************/
    /* TUPLE TEST FIXTURE                                                   */
    /************************************************************************/

    /// \brief Tuple test fixture.
    struct TupleTestFixture
    {
        /// \brief Default constructible struct definition.
        struct DefaultConstructibleFoo
        {
            DefaultConstructibleFoo() = default;

            DefaultConstructibleFoo(Float) {};
        };

        /// \brief Explicit default constructor struct definition.
        struct ExplicitDefaultConstructibleFoo
        {
            explicit ExplicitDefaultConstructibleFoo() {};
        };

        /// \brief Implicit default constructor struct definition.
        struct ImplicitDefaultConstructibleFoo
        {
            ImplicitDefaultConstructibleFoo() {};
        };

        /// \brief Optionally implicit default constructor struct definition.
        struct OptionalImplicitDefaultConstructibleFoo
        {
            OptionalImplicitDefaultConstructibleFoo(Int = 0) {};
        };

        /// \brief Optionally explicit default constructor struct definition.
        struct OptionalExplicitDefaultConstructibleFoo
        {
            explicit OptionalExplicitDefaultConstructibleFoo(Int = 0) {};
        };
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    template <typename TType>
    void CopyConstruct(const TType&) {};

    using TTypeList = Syntropy::Templates::TypeList<TupleTestFixture::ImplicitDefaultConstructibleFoo>;

    inline const auto& alignment_unit_test = MakeAutoUnitTest<TupleTestFixture>("tuple.experimental.foundation.core.syntropy")

    .TestCase("Empty tuples are always implicitly-default-constructible.", [](auto& fixture)
    {
        Tuple<> t{};
    })

    .TestCase("The default constructor of a tuple of default-constructible elements is explicit if at least one of those types' default constructor is explicit.", [](auto& fixture)
    {
        using ExplicitDefaultConstructibleFoo = TupleTestFixture::ExplicitDefaultConstructibleFoo;
        using ImplicitDefaultConstructibleFoo = TupleTestFixture::ImplicitDefaultConstructibleFoo;
        using OptionalExplicitDefaultConstructibleFoo = TupleTestFixture::OptionalExplicitDefaultConstructibleFoo;
        using OptionalImplicitDefaultConstructibleFoo = TupleTestFixture::OptionalImplicitDefaultConstructibleFoo;

        Tuple<ExplicitDefaultConstructibleFoo> t{};
        Tuple<ExplicitDefaultConstructibleFoo, ImplicitDefaultConstructibleFoo> u{};
        Tuple<OptionalExplicitDefaultConstructibleFoo> v{};
        Tuple<OptionalExplicitDefaultConstructibleFoo, OptionalImplicitDefaultConstructibleFoo> z{};
    })

    .TestCase("The default constructor of a tuple of default-constructible elements is implicit if all types' default constructors are implicit.", [](auto& fixture)
    {
        using ImplicitDefaultConstructibleFoo = TupleTestFixture::ImplicitDefaultConstructibleFoo;
        using OptionalImplicitDefaultConstructibleFoo = TupleTestFixture::OptionalImplicitDefaultConstructibleFoo;

        Tuple<ImplicitDefaultConstructibleFoo> t = {};
        Tuple<ImplicitDefaultConstructibleFoo, ImplicitDefaultConstructibleFoo> u = {};
        Tuple<OptionalImplicitDefaultConstructibleFoo> v = {};
        Tuple<OptionalImplicitDefaultConstructibleFoo, OptionalImplicitDefaultConstructibleFoo> z = {};
    });

}