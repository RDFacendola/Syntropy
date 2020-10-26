/// \file tuple_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <tuple>

#include "syntropy/language/foundation.h"
#include "syntropy/language/traits.h"
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

        /// \brief Explicit copy-constructible struct definition.
        struct ExplicitCopyConstructibleFoo
        {
            ExplicitCopyConstructibleFoo() {};
            explicit ExplicitCopyConstructibleFoo(const ExplicitCopyConstructibleFoo&) {};
        };

        /// \brief Implicit copy-constructible struct definition.
        struct ImplicitCopyConstructibleFoo
        {
            ImplicitCopyConstructibleFoo() {};
            ImplicitCopyConstructibleFoo(const ImplicitCopyConstructibleFoo&) {};
        };

        /// \brief Explicit move-constructible struct definition.
        struct ExplicitMoveConstructibleFoo
        {
            ExplicitMoveConstructibleFoo() {};
            explicit ExplicitMoveConstructibleFoo(ExplicitMoveConstructibleFoo&&) {};
        };

        /// \brief Implicit move-constructible struct definition.
        struct ImplicitMoveConstructibleFoo
        {
            ImplicitMoveConstructibleFoo() {};
            ImplicitMoveConstructibleFoo(ImplicitMoveConstructibleFoo&&) {};
        };

        /// \brief Type both ExplicitCopyConstructibleFoo and ImplicitCopyConstructibleFoo can be explicitly-converted to.
        struct ExplicitlyConvertibleFoo
        {
            explicit ExplicitlyConvertibleFoo(const ExplicitCopyConstructibleFoo&) {};
            explicit ExplicitlyConvertibleFoo(const ImplicitCopyConstructibleFoo&) {};

            explicit ExplicitlyConvertibleFoo(ExplicitMoveConstructibleFoo&&) {};
            explicit ExplicitlyConvertibleFoo(ImplicitMoveConstructibleFoo&&) {};
        };

        /// \brief Type both ExplicitCopyConstructibleFoo and ImplicitCopyConstructibleFoo can be implicitly-converted to.
        struct ImplicitlyConvertibleFoo
        {
            ImplicitlyConvertibleFoo(const ExplicitCopyConstructibleFoo&) {};
            ImplicitlyConvertibleFoo(const ImplicitCopyConstructibleFoo&) {};

            ImplicitlyConvertibleFoo(ExplicitMoveConstructibleFoo&&) {};
            ImplicitlyConvertibleFoo(ImplicitMoveConstructibleFoo&&) {};
        };
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& alignment_unit_test = MakeAutoUnitTest<TupleTestFixture>("tuple.experimental.foundation.core.syntropy")

    .TestCase("Empty tuples are always implicitly-default-constructible.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Syntropy::Templates::IsImplicitlyDefaultConstructible<Tuple<>>, true);
    })

    .TestCase("A tuple is explicitly default constructible if any of its elements is explicitly default-constructible.", [](auto& fixture)
    {
        using ExplicitDefaultConstructibleFoo = TupleTestFixture::ExplicitDefaultConstructibleFoo;
        using ImplicitDefaultConstructibleFoo = TupleTestFixture::ImplicitDefaultConstructibleFoo;
        using OptionalExplicitDefaultConstructibleFoo = TupleTestFixture::OptionalExplicitDefaultConstructibleFoo;
        using OptionalImplicitDefaultConstructibleFoo = TupleTestFixture::OptionalImplicitDefaultConstructibleFoo;

        // [i] Explicit default ctor: Tuple<T> t{};

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<ExplicitDefaultConstructibleFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsDefaultConstructible<ExplicitDefaultConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<Syntropy::Templates::TypeList<ExplicitDefaultConstructibleFoo, ImplicitDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsDefaultConstructible<Syntropy::Templates::TypeList<ExplicitDefaultConstructibleFoo, ImplicitDefaultConstructibleFoo>>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<OptionalExplicitDefaultConstructibleFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsDefaultConstructible<OptionalExplicitDefaultConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<Syntropy::Templates::TypeList<OptionalExplicitDefaultConstructibleFoo, OptionalImplicitDefaultConstructibleFoo>>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsDefaultConstructible<Syntropy::Templates::TypeList<OptionalExplicitDefaultConstructibleFoo, OptionalImplicitDefaultConstructibleFoo>>), true);
    })

    .TestCase("A tuple is implicitly default constructible if all of its elements are explicitly default-constructible.", [](auto& fixture)
    {
        using ExplicitDefaultConstructibleFoo = TupleTestFixture::ExplicitDefaultConstructibleFoo;
        using ImplicitDefaultConstructibleFoo = TupleTestFixture::ImplicitDefaultConstructibleFoo;
        using OptionalExplicitDefaultConstructibleFoo = TupleTestFixture::OptionalExplicitDefaultConstructibleFoo;
        using OptionalImplicitDefaultConstructibleFoo = TupleTestFixture::OptionalImplicitDefaultConstructibleFoo;

        // [i] Implicit default ctor: Tuple<T> t = {};

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<ImplicitDefaultConstructibleFoo>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsDefaultConstructible<ImplicitDefaultConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<Syntropy::Templates::TypeList<ImplicitDefaultConstructibleFoo, ImplicitDefaultConstructibleFoo>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsDefaultConstructible<Syntropy::Templates::TypeList<ImplicitDefaultConstructibleFoo, ImplicitDefaultConstructibleFoo>>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<OptionalImplicitDefaultConstructibleFoo>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsDefaultConstructible<OptionalImplicitDefaultConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyDefaultConstructible<Syntropy::Templates::TypeList<OptionalImplicitDefaultConstructibleFoo, OptionalImplicitDefaultConstructibleFoo>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsDefaultConstructible<Syntropy::Templates::TypeList<OptionalImplicitDefaultConstructibleFoo, OptionalImplicitDefaultConstructibleFoo>>), true);
    })

    .TestCase("A tuple is explicitly direct-constructible if any of its elements is explicitly direct-constructible.", [](auto& fixture)
    {
        using ExplicitCopyConstructibleFoo = TupleTestFixture::ExplicitCopyConstructibleFoo;
        using ImplicitCopyConstructibleFoo = TupleTestFixture::ImplicitCopyConstructibleFoo;
        using ExplicitlyConvertibleFoo = TupleTestFixture::ExplicitlyConvertibleFoo;

        // [i] Explicit direct ctor: Tuple<U> t{u};

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ExplicitCopyConstructibleFoo>, ExplicitCopyConstructibleFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ExplicitCopyConstructibleFoo>, ExplicitCopyConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>, ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>, ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ExplicitlyConvertibleFoo>, ImplicitCopyConstructibleFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ExplicitlyConvertibleFoo>, ImplicitCopyConstructibleFoo>), true);
    })

    .TestCase("A tuple is implicitly direct-constructible if all of its elements are implicitly direct-constructible.", [](auto& fixture)
    {
        using ExplicitCopyConstructibleFoo = TupleTestFixture::ExplicitCopyConstructibleFoo;
        using ImplicitCopyConstructibleFoo = TupleTestFixture::ImplicitCopyConstructibleFoo;
        using ImplicitlyConvertibleFoo = TupleTestFixture::ImplicitlyConvertibleFoo;

        // [i] Implicit direct ctor: Tuple<U> t = {u};

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ImplicitCopyConstructibleFoo>, ImplicitCopyConstructibleFoo>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ImplicitCopyConstructibleFoo>, ImplicitCopyConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ImplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>, ImplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ImplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>, ImplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ImplicitlyConvertibleFoo>, ExplicitCopyConstructibleFoo>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ImplicitlyConvertibleFoo>, ExplicitCopyConstructibleFoo>), true);
     })

    .TestCase("A tuple is explicitly converting-copy-constructible if any of its elements is explicitly direct-constructible or copy-constructible.", [](auto& fixture)
    {
        using ExplicitCopyConstructibleFoo = TupleTestFixture::ExplicitCopyConstructibleFoo;
        using ImplicitCopyConstructibleFoo = TupleTestFixture::ImplicitCopyConstructibleFoo;
        using ExplicitlyConvertibleFoo = TupleTestFixture::ExplicitlyConvertibleFoo;
        using ImplicitlyConvertibleFoo = TupleTestFixture::ImplicitlyConvertibleFoo;

        // [i] Explicit converting-copy ctor: Tuple<U> t { Tuple<V>(v) };

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ExplicitlyConvertibleFoo>, const Tuple<ExplicitCopyConstructibleFoo>&>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ExplicitlyConvertibleFoo>, const Tuple<ExplicitCopyConstructibleFoo>&>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ExplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, const Tuple<ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>&>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ExplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, const Tuple<ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>>), true);
     })

    .TestCase("A tuple is implicitly converting-copy-constructible if all of its elements are explicitly direct-constructible or copy-constructible.", [](auto& fixture)
    {
        using ExplicitCopyConstructibleFoo = TupleTestFixture::ExplicitCopyConstructibleFoo;
        using ImplicitCopyConstructibleFoo = TupleTestFixture::ImplicitCopyConstructibleFoo;
        using ImplicitlyConvertibleFoo = TupleTestFixture::ImplicitlyConvertibleFoo;

        // [i] Implicit converting-copy ctor: Tuple<U> t = { Tuple<V>(v) };

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ImplicitlyConvertibleFoo>, const Tuple<ImplicitCopyConstructibleFoo>&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ImplicitlyConvertibleFoo>, const Tuple<ImplicitCopyConstructibleFoo>&>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ImplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, const Tuple<ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ImplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, const Tuple<ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>&>), true);
     })

    .TestCase("A tuple is explicitly converting-move-constructible if any of its elements is explicitly direct-constructible or move-constructible.", [](auto& fixture)
    {
        using ExplicitMoveConstructibleFoo = TupleTestFixture::ExplicitMoveConstructibleFoo;
        using ImplicitMoveConstructibleFoo = TupleTestFixture::ImplicitMoveConstructibleFoo;
        using ExplicitlyConvertibleFoo = TupleTestFixture::ExplicitlyConvertibleFoo;
        using ImplicitlyConvertibleFoo = TupleTestFixture::ImplicitlyConvertibleFoo;

        // [i] Explicit converting-copy ctor: Tuple<U> t { Tuple<V>(v) };

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ExplicitlyConvertibleFoo>, Tuple<ExplicitMoveConstructibleFoo>&&>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ExplicitlyConvertibleFoo>, Tuple<ExplicitMoveConstructibleFoo>&&>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ExplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, Tuple<ExplicitMoveConstructibleFoo, ImplicitMoveConstructibleFoo>&&>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ExplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, Tuple<ExplicitMoveConstructibleFoo, ImplicitMoveConstructibleFoo>&&>), true);
     })

    .TestCase("A tuple is implicitly converting-move-constructible if all of its elements are explicitly direct-constructible or move-constructible.", [](auto& fixture)
    {
        using ExplicitMoveConstructibleFoo = TupleTestFixture::ExplicitMoveConstructibleFoo;
        using ImplicitMoveConstructibleFoo = TupleTestFixture::ImplicitMoveConstructibleFoo;
        using ImplicitlyConvertibleFoo = TupleTestFixture::ImplicitlyConvertibleFoo;

        // [i] Implicit converting-copy ctor: Tuple<U> t = { Tuple<V>(v) };

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ImplicitlyConvertibleFoo>, Tuple<ImplicitMoveConstructibleFoo>&&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ImplicitlyConvertibleFoo>, Tuple<ImplicitMoveConstructibleFoo>&&>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<Tuple<ImplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, Tuple<ExplicitMoveConstructibleFoo, ImplicitMoveConstructibleFoo>&&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<Tuple<ImplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, Tuple<ExplicitMoveConstructibleFoo, ImplicitMoveConstructibleFoo>&&>), true);
     })

    .TestCase("A tuple size is equal to the number of elements it can store.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::TupleSize<Tuple<>>), 0);
        SYNTROPY_UNIT_EQUAL((Templates::TupleSize<Tuple<Int>>), 1);
        SYNTROPY_UNIT_EQUAL((Templates::TupleSize<Tuple<Int, Float>>), 2);
    })

    .TestCase("TupleElement trait is used to get the type a tuple element by index.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<Templates::TupleElement<0, Tuple<Bool>>, Bool>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<Templates::TupleElement<0, Tuple<Int, Float>>, Int>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<Templates::TupleElement<1, Tuple<Int, Float>>, Float>), true);
    });

}