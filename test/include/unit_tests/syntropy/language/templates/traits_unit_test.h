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
        struct Bar {};

        struct Foo {};

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

        struct ConstructibleFromBar
        {
            ConstructibleFromBar(const Bar&) {};
        };

        struct ConstructibleFromFoo
        {
            ConstructibleFromFoo(const Foo&) {};
        };

        struct ConstructibleFromFooAndBar
        {
            ConstructibleFromFooAndBar(const Foo&, const Bar&) {};
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
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<>>), true);
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
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreImplicitlyDefaultConstructible<TypeList<>>), true);
    })

    .TestCase("Are copy-constructible type-trait.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreCopyConstructible<TypeList<TraitsTestFixture::NonCopyableFoo, TraitsTestFixture::CopyableFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreCopyConstructible<TypeList<TraitsTestFixture::CopyableFoo, TraitsTestFixture::NonCopyableFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreCopyConstructible<TypeList<TraitsTestFixture::CopyableFoo, TraitsTestFixture::CopyableFoo>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreCopyConstructible<TypeList<>>), true);
    })

    .TestCase("Are constructible type-traits", [](auto& fixture)
    {
        using Foo = TraitsTestFixture::Foo;
        using Bar = TraitsTestFixture::Bar;
        using ConstructibleFromFoo = TraitsTestFixture::ConstructibleFromFoo;
        using ConstructibleFromBar = TraitsTestFixture::ConstructibleFromBar;
        using ConstructibleFromFooAndBar = TraitsTestFixture::ConstructibleFromFooAndBar;

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConstructible<TypeList<>, TypeList<>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConstructible<TypeList<>, TypeList<Int>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConstructible<TypeList<Float>, TypeList<>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConstructible<TypeList<Float>, TypeList<TypeList<Int>>>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConstructible<TypeList<ConstructibleFromFoo, ConstructibleFromBar, ConstructibleFromFooAndBar>, TypeList<TypeList<Foo>, TypeList<Bar>, TypeList<Foo, Bar>>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConstructible<TypeList<ConstructibleFromFoo, ConstructibleFromBar, ConstructibleFromFooAndBar>, TypeList<TypeList<Bar>, TypeList<Foo, Bar>, TypeList<Foo>>>), false);
    });
}