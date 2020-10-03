#/// \file traits_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/traits.h"

#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::Templates::UnitTest
{
    /************************************************************************/
    /* TRAITS TEST FIXTURE                                                  */
    /************************************************************************/

    /// \brief Traits test fixture.
    struct TraitsTestFixture
    {
        struct Foo
        {
            Foo(Int) {};
        };

        struct Bar
        {
            Bar(Float) {};
        };

        struct ExplicitFoo
        {
            explicit ExplicitFoo() {};
        };

        struct ImplicitFoo
        {
            ImplicitFoo() {};
        };
    };

    /************************************************************************/
    /* UNIT TESTS                                                           */
    /************************************************************************/

    inline const auto& kTraitsUnitTest = MakeAutoUnitTest<TraitsTestFixture>("traits.templates.language.syntropy")

    .TestCase("Are default constructible type-trait.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<int>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<float, float, float>>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<TraitsTestFixture::Foo, TraitsTestFixture::Bar>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<TraitsTestFixture::Foo, TraitsTestFixture::Bar, int>>), false);
    })

    .TestCase("Is implicitly default constructible type-trait.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<TraitsTestFixture::ExplicitFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<TraitsTestFixture::ImplicitFoo>), true);
    })

    .TestCase("Are implicitly default constructible type-trait.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreImplicitlyDefaultConstructible<TypeList<TraitsTestFixture::ExplicitFoo, TraitsTestFixture::ImplicitFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreImplicitlyDefaultConstructible<TypeList<TraitsTestFixture::ImplicitFoo, TraitsTestFixture::ImplicitFoo>>), true);
    });
}