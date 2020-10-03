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
    };

    /************************************************************************/
    /* UNIT TESTS                                                           */
    /************************************************************************/

    inline const auto& kTraitsUnitTest = MakeAutoUnitTest<TraitsTestFixture>("traits.templates.language.syntropy")

    .TestCase("Test", [](auto& fixture)
    {
        // Ill-formed commented out.

        // auto r00 = Syntropy::Templates::AreDefaultConstructible<float>;
        // auto r01 = Syntropy::Templates::AreDefaultConstructible<float, int>;

        // SYNTROPY_UNIT_EQUAL(Syntropy::Templates::AreDefaultConstructible<TypeList<>>, true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<int>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<float, float, float>>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<TraitsTestFixture::Foo, TraitsTestFixture::Bar>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<TraitsTestFixture::Foo, TraitsTestFixture::Bar, int>>), false);
    });

}