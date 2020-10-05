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
        /// \brief Generic struct definition.
        struct Foo {};

        /// \brief Generic struct definition.
        struct Bar {};

        /// \brief Non-default constructible struct definition.
        struct NonDefaultConstructibleFoo
        {
            NonDefaultConstructibleFoo() = delete;

            NonDefaultConstructibleFoo(Int) {};
        };

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

        /// \brief Copyable struct definition.
        struct CopyableFoo
        {
            CopyableFoo(const CopyableFoo&) = default;
        };

        /// \brief Non-copyable struct definition.
        struct NonCopyableFoo
        {
            NonCopyableFoo(const NonCopyableFoo&) = delete;
        };

        /// \brief Definition for a class that can be constructed from Foo.
        struct ConstructibleFromFoo
        {
            ConstructibleFromFoo(const Foo&) {};
        };

        /// \brief Definition for a class that can be constructed from Bar.
        struct ConstructibleFromBar
        {
            ConstructibleFromBar(const Bar&) {};
        };

        /// \brief Definition for a class that can be constructed from Foo and Bar.
        struct ConstructibleFromFooAndBar
        {
            ConstructibleFromFooAndBar(const Foo&, const Bar&) {};
        };

        /// \brief Definition for a class that can be converted to Bar.
        struct ConvertibleToBar : public Bar
        {

        };

        /// \brief Definition for a class that can be converted to Foo.
        struct ConvertibleToFoo : public Foo
        {

        };
    };

    /************************************************************************/
    /* UNIT TESTS                                                           */
    /************************************************************************/

    inline const auto& kTraitsUnitTest = MakeAutoUnitTest<TraitsTestFixture>("traits.templates.language.syntropy")

    .TestCase("Are default-constructible type-trait.", [](auto& fixture)
    {
        using NonDefaultConstructibleFoo = TraitsTestFixture::NonDefaultConstructibleFoo;
        using DefaultConstructibleFoo = TraitsTestFixture::DefaultConstructibleFoo;

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<NonDefaultConstructibleFoo, DefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<DefaultConstructibleFoo, NonDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<DefaultConstructibleFoo, DefaultConstructibleFoo>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreDefaultConstructible<TypeList<>>), true);
    })

    .TestCase("Is implicitly default-constructible type-trait.", [](auto& fixture)
    {
        using ExplicitDefaultConstructibleFoo = TraitsTestFixture::ExplicitDefaultConstructibleFoo;
        using ImplicitDefaultConstructibleFoo = TraitsTestFixture::ImplicitDefaultConstructibleFoo;
        using OptionalExplicitDefaultConstructibleFoo = TraitsTestFixture::OptionalExplicitDefaultConstructibleFoo;
        using OptionalImplicitDefaultConstructibleFoo = TraitsTestFixture::OptionalImplicitDefaultConstructibleFoo;

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<ExplicitDefaultConstructibleFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<ImplicitDefaultConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<OptionalExplicitDefaultConstructibleFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<OptionalImplicitDefaultConstructibleFoo>), true);
    })

    .TestCase("Are implicitly default-constructible type-trait.", [](auto& fixture)
    {
        using ExplicitDefaultConstructibleFoo = TraitsTestFixture::ExplicitDefaultConstructibleFoo;
        using ImplicitDefaultConstructibleFoo = TraitsTestFixture::ImplicitDefaultConstructibleFoo;
        using OptionalExplicitDefaultConstructibleFoo = TraitsTestFixture::OptionalExplicitDefaultConstructibleFoo;
        using OptionalImplicitDefaultConstructibleFoo = TraitsTestFixture::OptionalImplicitDefaultConstructibleFoo;

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreImplicitlyDefaultConstructible<TypeList<ExplicitDefaultConstructibleFoo, ImplicitDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreImplicitlyDefaultConstructible<TypeList<ImplicitDefaultConstructibleFoo, ExplicitDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreImplicitlyDefaultConstructible<TypeList<ImplicitDefaultConstructibleFoo, ImplicitDefaultConstructibleFoo>>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreImplicitlyDefaultConstructible<TypeList<OptionalExplicitDefaultConstructibleFoo, OptionalImplicitDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreImplicitlyDefaultConstructible<TypeList<OptionalImplicitDefaultConstructibleFoo, OptionalExplicitDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreImplicitlyDefaultConstructible<TypeList<OptionalImplicitDefaultConstructibleFoo, OptionalImplicitDefaultConstructibleFoo>>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreImplicitlyDefaultConstructible<TypeList<>>), true);
    })

    .TestCase("Are copy-constructible type-trait.", [](auto& fixture)
    {
        using NonCopyableFoo = TraitsTestFixture::NonCopyableFoo;
        using CopyableFoo = TraitsTestFixture::CopyableFoo;

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreCopyConstructible<TypeList<NonCopyableFoo, CopyableFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreCopyConstructible<TypeList<CopyableFoo, NonCopyableFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreCopyConstructible<TypeList<CopyableFoo, CopyableFoo>>), true);
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
    })

    .TestCase("Are constructible type-traits", [](auto& fixture)
    {
        using Foo = TraitsTestFixture::Foo;
        using Bar = TraitsTestFixture::Bar;
        using ConstructibleFromFoo = TraitsTestFixture::ConstructibleFromFoo;
        using ConstructibleFromBar = TraitsTestFixture::ConstructibleFromBar;
        using ConvertibleToFoo = TraitsTestFixture::ConvertibleToFoo;
        using ConvertibleToBar = TraitsTestFixture::ConvertibleToBar;

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConvertible<TypeList<>, TypeList<>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConvertible<TypeList<Int>, TypeList<Int, Float>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConvertible<TypeList<Int>, TypeList<Float>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConvertible<TypeList<Float>, TypeList<Int>>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConvertible<TypeList<>, TypeList<>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConvertible<TypeList<>, TypeList<Int>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConvertible<TypeList<Float>, TypeList<>>), false);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConvertible<TypeList<ConvertibleToFoo, ConvertibleToBar>, TypeList<Foo, Bar>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConvertible<TypeList<ConvertibleToFoo, ConvertibleToBar>, TypeList<Bar, Foo>>), false);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConvertible<TypeList<Foo, Bar>, TypeList<ConstructibleFromFoo, ConstructibleFromBar>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::AreConvertible<TypeList<Foo, Bar>, TypeList<ConstructibleFromBar, ConstructibleFromFoo>>), false);
    });
}