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
        struct NonDefaultConstructibleFoo
        {
            NonDefaultConstructibleFoo() = delete;

            NonDefaultConstructibleFoo(Int) {};
        };

        struct DefaultConstructibleFoo
        {
            DefaultConstructibleFoo() = default;

            DefaultConstructibleFoo(Float) {};
        };

        struct ExplicitDefaultConstructibleFoo
        {
            explicit ExplicitDefaultConstructibleFoo() {};
        };

        struct ImplicitDefaultConstructibleFoo
        {
            ImplicitDefaultConstructibleFoo() {};
        };

        struct CopyableFoo
        {
            CopyableFoo(const CopyableFoo&) = default;
        };

        struct NonCopyableFoo
        {
            NonCopyableFoo(const NonCopyableFoo&) = delete;
        };
    };

    /************************************************************************/
    /* UNIT TESTS                                                           */
    /************************************************************************/

    inline const auto& kTraitsUnitTest = MakeAutoUnitTest<TraitsTestFixture>("traits.templates.language.syntropy")

    .TestCase("Are default-constructible type-trait.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<TraitsTestFixture::NonDefaultConstructibleFoo, TraitsTestFixture::DefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<TraitsTestFixture::DefaultConstructibleFoo, TraitsTestFixture::NonDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<TraitsTestFixture::DefaultConstructibleFoo, TraitsTestFixture::DefaultConstructibleFoo>>), true);
    })

    .TestCase("Is implicitly default-constructible type-trait.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<TraitsTestFixture::ExplicitDefaultConstructibleFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<TraitsTestFixture::ImplicitDefaultConstructibleFoo>), true);
    })

    .TestCase("Are implicitly default-constructible type-trait.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreImplicitlyDefaultConstructible<TypeList<TraitsTestFixture::ExplicitDefaultConstructibleFoo, TraitsTestFixture::ImplicitDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreImplicitlyDefaultConstructible<TypeList<TraitsTestFixture::ImplicitDefaultConstructibleFoo, TraitsTestFixture::ExplicitDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreImplicitlyDefaultConstructible<TypeList<TraitsTestFixture::ImplicitDefaultConstructibleFoo, TraitsTestFixture::ImplicitDefaultConstructibleFoo>>), true);
    })

    .TestCase("Are copy-constructible type-trait.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreCopyConstructible<TypeList<TraitsTestFixture::NonCopyableFoo, TraitsTestFixture::CopyableFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreCopyConstructible<TypeList<TraitsTestFixture::CopyableFoo, TraitsTestFixture::NonCopyableFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreCopyConstructible<TypeList<TraitsTestFixture::CopyableFoo, TraitsTestFixture::CopyableFoo>>), true);
    });
}