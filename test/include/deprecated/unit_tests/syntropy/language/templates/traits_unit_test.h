#/// \file traits_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/traits.h"

#include "syntropy/unit_test/unit_test.h"

// ===========================================================================

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

        /// \brief Definition for a class that can be copy-assigned from foo.
        struct CopyAssignableFromFoo
        {
            CopyAssignableFromFoo(const Foo&) = delete;

            CopyAssignableFromFoo(Foo&&) = delete;

            CopyAssignableFromFoo& operator=(const Foo&) { return *this; };

            CopyAssignableFromFoo& operator=(Foo&&) = delete;
        };

        /// \brief Definition for a class that can be move-assigned from foo.
        struct MoveAssignableFromFoo
        {
            MoveAssignableFromFoo(const Foo&) = delete;

            MoveAssignableFromFoo(Foo&&) = delete;

            MoveAssignableFromFoo& operator=(const Foo&) = delete;

            MoveAssignableFromFoo& operator=(Foo&&) { return *this; };
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

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsDefaultConstructible<TypeList<NonDefaultConstructibleFoo, DefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsDefaultConstructible<TypeList<DefaultConstructibleFoo, NonDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsDefaultConstructible<TypeList<DefaultConstructibleFoo, DefaultConstructibleFoo>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsDefaultConstructible<TypeList<>>), true);
    })

    .TestCase("Is implicitly default-constructible type-trait.", [](auto& fixture)
    {
        using ExplicitDefaultConstructibleFoo = TraitsTestFixture::ExplicitDefaultConstructibleFoo;
        using ImplicitDefaultConstructibleFoo = TraitsTestFixture::ImplicitDefaultConstructibleFoo;
        using OptionalExplicitDefaultConstructibleFoo = TraitsTestFixture::OptionalExplicitDefaultConstructibleFoo;
        using OptionalImplicitDefaultConstructibleFoo = TraitsTestFixture::OptionalImplicitDefaultConstructibleFoo;

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsImplicitlyDefaultConstructible<ExplicitDefaultConstructibleFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsImplicitlyDefaultConstructible<ImplicitDefaultConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsImplicitlyDefaultConstructible<OptionalExplicitDefaultConstructibleFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsImplicitlyDefaultConstructible<OptionalImplicitDefaultConstructibleFoo>), true);
    })

    .TestCase("Are implicitly default-constructible type-trait.", [](auto& fixture)
    {
        using ExplicitDefaultConstructibleFoo = TraitsTestFixture::ExplicitDefaultConstructibleFoo;
        using ImplicitDefaultConstructibleFoo = TraitsTestFixture::ImplicitDefaultConstructibleFoo;
        using OptionalExplicitDefaultConstructibleFoo = TraitsTestFixture::OptionalExplicitDefaultConstructibleFoo;
        using OptionalImplicitDefaultConstructibleFoo = TraitsTestFixture::OptionalImplicitDefaultConstructibleFoo;

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsImplicitlyDefaultConstructible<TypeList<ExplicitDefaultConstructibleFoo, ImplicitDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsImplicitlyDefaultConstructible<TypeList<ImplicitDefaultConstructibleFoo, ExplicitDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsImplicitlyDefaultConstructible<TypeList<ImplicitDefaultConstructibleFoo, ImplicitDefaultConstructibleFoo>>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsImplicitlyDefaultConstructible<TypeList<OptionalExplicitDefaultConstructibleFoo, OptionalImplicitDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsImplicitlyDefaultConstructible<TypeList<OptionalImplicitDefaultConstructibleFoo, OptionalExplicitDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsImplicitlyDefaultConstructible<TypeList<OptionalImplicitDefaultConstructibleFoo, OptionalImplicitDefaultConstructibleFoo>>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsImplicitlyDefaultConstructible<TypeList<>>), true);
    })

    .TestCase("Are copy-constructible type-trait.", [](auto& fixture)
    {
        using NonCopyableFoo = TraitsTestFixture::NonCopyableFoo;
        using CopyableFoo = TraitsTestFixture::CopyableFoo;

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsCopyConstructible<TypeList<NonCopyableFoo, CopyableFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsCopyConstructible<TypeList<CopyableFoo, NonCopyableFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsCopyConstructible<TypeList<CopyableFoo, CopyableFoo>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsCopyConstructible<TypeList<>>), true);
    })

    .TestCase("Are constructible type-traits", [](auto& fixture)
    {
        using Foo = TraitsTestFixture::Foo;
        using Bar = TraitsTestFixture::Bar;
        using ConstructibleFromFoo = TraitsTestFixture::ConstructibleFromFoo;
        using ConstructibleFromBar = TraitsTestFixture::ConstructibleFromBar;
        using ConstructibleFromFooAndBar = TraitsTestFixture::ConstructibleFromFooAndBar;

        static_assert(std::is_constructible_v<float>, "nope!");

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::ConstructibleType<TypeList<>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::ConstructibleType<TypeList<Float>, TypeList<>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::ConstructibleType<TypeList<Float>, TypeList<Int>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::ConstructibleType<TypeList<Int>, TypeList<Float>>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::ConstructibleType<TypeList<Float, Int>, TypeList<>, TypeList<>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::ConstructibleType<TypeList<Float, Int>, TypeList<Foo>, TypeList<>>), false);

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::ConstructibleType<TypeList<ConstructibleFromFoo, ConstructibleFromBar, ConstructibleFromFooAndBar>, TypeList<Foo>, TypeList<Bar>, TypeList<Foo, Bar>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::ConstructibleType<TypeList<ConstructibleFromFoo, ConstructibleFromBar, ConstructibleFromFooAndBar>, TypeList<Bar>, TypeList<Foo, Bar>, TypeList<Foo>>), false);
    })

    .TestCase("Are constructible type-traits", [](auto& fixture)
    {
        using Foo = TraitsTestFixture::Foo;
        using Bar = TraitsTestFixture::Bar;
        using ConstructibleFromFoo = TraitsTestFixture::ConstructibleFromFoo;
        using ConstructibleFromBar = TraitsTestFixture::ConstructibleFromBar;
        using ConvertibleToFoo = TraitsTestFixture::ConvertibleToFoo;
        using ConvertibleToBar = TraitsTestFixture::ConvertibleToBar;

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsConvertible<TypeList<>, TypeList<>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsConvertible<TypeList<Int>, TypeList<Float>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsConvertible<TypeList<Float>, TypeList<Int>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsConvertible<Int, Float>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsConvertible<Float, Int>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsConvertible<TypeList<ConvertibleToFoo, ConvertibleToBar>, TypeList<Foo, Bar>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsConvertible<TypeList<ConvertibleToFoo, ConvertibleToBar>, TypeList<Bar, Foo>>), false);

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsConvertible<TypeList<Foo, Bar>, TypeList<ConstructibleFromFoo, ConstructibleFromBar>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsConvertible<TypeList<Foo, Bar>, TypeList<ConstructibleFromBar, ConstructibleFromFoo>>), false);
        })

    .TestCase("Is assignable type-traits.", [](auto& fixture)
    {
        using Foo = TraitsTestFixture::Foo;
        using Bar = TraitsTestFixture::Bar;
        using CopyAssignableFromFoo = TraitsTestFixture::CopyAssignableFromFoo;
        using MoveAssignableFromFoo = TraitsTestFixture::MoveAssignableFromFoo;

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::AssignableType<CopyAssignableFromFoo, const Foo&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::AssignableType<CopyAssignableFromFoo, const Bar&>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::AssignableType<CopyAssignableFromFoo, Foo&&>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::AssignableType<CopyAssignableFromFoo, Bar&&>), false);

        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::AssignableType<MoveAssignableFromFoo, const Foo&>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::AssignableType<MoveAssignableFromFoo, const Bar&>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::AssignableType<MoveAssignableFromFoo, Foo&&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::AssignableType<MoveAssignableFromFoo, Bar&&>), false);
    })
            
    .TestCase("IndexOf returns the index of the first occurrence in a type list.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((IndexOf<Int, TypeList<Int, Float, Bool>>), 0);
        SYNTROPY_UNIT_EQUAL((IndexOf<Int, TypeList<Float, Bool, Int>>), 2);
    })

    .TestCase("ElementOf returns the type of an element in a type list by index.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsSame<ElementOf<0, TypeList<Int, Float, Bool>>, Int>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsSame<ElementOf<1, TypeList<Int, Float, Bool>>, Float>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsSame<ElementOf<2, TypeList<Int, Float, Bool>>, Bool>), true);
    })

    .TestCase("Removing elements from a type list by means of Drop return a new type list which is equal to the remaining elements in the original type list.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsSame<Drop<0, TypeList<Int, Float, Bool>>, TypeList<Int, Float, Bool>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsSame<Drop<1, TypeList<Int, Float, Bool>>, TypeList<Float, Bool>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsSame<Drop<2, TypeList<Int, Float, Bool>>, TypeList<Bool>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Concepts::IsSame<Drop<3, TypeList<Int, Float, Bool>>, TypeList<>>), true);
    });

}

// ===========================================================================