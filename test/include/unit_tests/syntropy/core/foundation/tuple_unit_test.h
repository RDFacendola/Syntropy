/// \file tuple_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include <tuple>

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/traits.h"
#include "syntropy/core/foundation/tuple.h"

#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::UnitTest
{
    /************************************************************************/
    /* TUPLE TEST FIXTURE                                                   */
    /************************************************************************/

    /// \brief TupleT test fixture.
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

        /// \brief Definition for a movable-only structure.
        struct MovableOnlyFoo
        {
            MovableOnlyFoo() = default;
            MovableOnlyFoo(MovableOnlyFoo&&) {};
            MovableOnlyFoo(const MovableOnlyFoo&) = delete;

            MovableOnlyFoo& operator=(MovableOnlyFoo&&) { return *this; };
            MovableOnlyFoo& operator=(const MovableOnlyFoo&) = delete;
        };

        /// \brief Definition for a copyable structure.
        struct CopyableOnlyFoo
        {
            CopyableOnlyFoo() = default;
            CopyableOnlyFoo(const CopyableOnlyFoo&) {};

            CopyableOnlyFoo& operator=(const CopyableOnlyFoo&) { return *this; };
        };

        /// \brief Movable class which verifies if it was moved from.
        struct TestMovableOnlyFoo
        {
            TestMovableOnlyFoo() = default;
            TestMovableOnlyFoo(TestMovableOnlyFoo&& rhs) { rhs.moved_ = true; }
            TestMovableOnlyFoo(const TestMovableOnlyFoo&) = delete;
            
            TestMovableOnlyFoo& operator=(TestMovableOnlyFoo&&) { return *this; };
            TestMovableOnlyFoo& operator=(const TestMovableOnlyFoo&) = delete;

            bool moved_{ false };
        };

        /// \brief Movable class which can be constructed by moving a TestMovableOnlyFoo instance.
        struct TestMovableOnlyBar
        {
            TestMovableOnlyBar() = default;
            TestMovableOnlyBar(TestMovableOnlyFoo&& rhs) { rhs.moved_ = true; }
        };
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& alignment_unit_test = MakeAutoUnitTest<TupleTestFixture>("tuple.experimental.foundation.core.syntropy")

    .TestCase("Empty tuples are always implicitly-default-constructible.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Syntropy::Templates::IsImplicitlyDefaultConstructible<TupleT<>>, true);
    })

    .TestCase("A tuple is explicitly default constructible if any of its elements is explicitly default-constructible.", [](auto& fixture)
    {
        using ExplicitDefaultConstructibleFoo = TupleTestFixture::ExplicitDefaultConstructibleFoo;
        using ImplicitDefaultConstructibleFoo = TupleTestFixture::ImplicitDefaultConstructibleFoo;
        using OptionalExplicitDefaultConstructibleFoo = TupleTestFixture::OptionalExplicitDefaultConstructibleFoo;
        using OptionalImplicitDefaultConstructibleFoo = TupleTestFixture::OptionalImplicitDefaultConstructibleFoo;

        // [i] Explicit default ctor: TupleT<T> t{};

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

        // [i] Implicit default ctor: TupleT<T> t = {};

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

        // [i] Explicit direct ctor: TupleT<U> t{u};

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ExplicitCopyConstructibleFoo>, ExplicitCopyConstructibleFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ExplicitCopyConstructibleFoo>, ExplicitCopyConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>, ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>, ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ExplicitlyConvertibleFoo>, ImplicitCopyConstructibleFoo>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ExplicitlyConvertibleFoo>, ImplicitCopyConstructibleFoo>), true);
    })

    .TestCase("A tuple is implicitly direct-constructible if all of its elements are implicitly direct-constructible.", [](auto& fixture)
    {
        using ExplicitCopyConstructibleFoo = TupleTestFixture::ExplicitCopyConstructibleFoo;
        using ImplicitCopyConstructibleFoo = TupleTestFixture::ImplicitCopyConstructibleFoo;
        using ImplicitlyConvertibleFoo = TupleTestFixture::ImplicitlyConvertibleFoo;

        // [i] Implicit direct ctor: TupleT<U> t = {u};

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ImplicitCopyConstructibleFoo>, ImplicitCopyConstructibleFoo>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ImplicitCopyConstructibleFoo>, ImplicitCopyConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ImplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>, ImplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ImplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>, ImplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ImplicitlyConvertibleFoo>, ExplicitCopyConstructibleFoo>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ImplicitlyConvertibleFoo>, ExplicitCopyConstructibleFoo>), true);
     })

    .TestCase("A tuple is explicitly converting-copy-constructible if any of its elements is explicitly direct-constructible or copy-constructible.", [](auto& fixture)
    {
        using ExplicitCopyConstructibleFoo = TupleTestFixture::ExplicitCopyConstructibleFoo;
        using ImplicitCopyConstructibleFoo = TupleTestFixture::ImplicitCopyConstructibleFoo;
        using ExplicitlyConvertibleFoo = TupleTestFixture::ExplicitlyConvertibleFoo;
        using ImplicitlyConvertibleFoo = TupleTestFixture::ImplicitlyConvertibleFoo;

        // [i] Explicit converting-copy ctor: TupleT<U> t { TupleT<V>(v) };

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ExplicitlyConvertibleFoo>, const TupleT<ExplicitCopyConstructibleFoo>&>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ExplicitlyConvertibleFoo>, const TupleT<ExplicitCopyConstructibleFoo>&>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ExplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, const TupleT<ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>&>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ExplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, const TupleT<ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>>), true);
     })

    .TestCase("A tuple is implicitly converting-copy-constructible if all of its elements are explicitly direct-constructible or copy-constructible.", [](auto& fixture)
    {
        using ExplicitCopyConstructibleFoo = TupleTestFixture::ExplicitCopyConstructibleFoo;
        using ImplicitCopyConstructibleFoo = TupleTestFixture::ImplicitCopyConstructibleFoo;
        using ImplicitlyConvertibleFoo = TupleTestFixture::ImplicitlyConvertibleFoo;

        // [i] Implicit converting-copy ctor: TupleT<U> t = { TupleT<V>(v) };

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ImplicitlyConvertibleFoo>, const TupleT<ImplicitCopyConstructibleFoo>&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ImplicitlyConvertibleFoo>, const TupleT<ImplicitCopyConstructibleFoo>&>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ImplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, const TupleT<ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ImplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, const TupleT<ExplicitCopyConstructibleFoo, ImplicitCopyConstructibleFoo>&>), true);
     })

    .TestCase("A tuple is explicitly converting-move-constructible if any of its elements is explicitly direct-constructible or move-constructible.", [](auto& fixture)
    {
        using ExplicitMoveConstructibleFoo = TupleTestFixture::ExplicitMoveConstructibleFoo;
        using ImplicitMoveConstructibleFoo = TupleTestFixture::ImplicitMoveConstructibleFoo;
        using ExplicitlyConvertibleFoo = TupleTestFixture::ExplicitlyConvertibleFoo;
        using ImplicitlyConvertibleFoo = TupleTestFixture::ImplicitlyConvertibleFoo;

        // [i] Explicit converting-copy ctor: TupleT<U> t { TupleT<V>(v) };

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ExplicitlyConvertibleFoo>, TupleT<ExplicitMoveConstructibleFoo>&&>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ExplicitlyConvertibleFoo>, TupleT<ExplicitMoveConstructibleFoo>&&>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ExplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, TupleT<ExplicitMoveConstructibleFoo, ImplicitMoveConstructibleFoo>&&>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ExplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, TupleT<ExplicitMoveConstructibleFoo, ImplicitMoveConstructibleFoo>&&>), true);
     })

    .TestCase("A tuple is implicitly converting-move-constructible if all of its elements are explicitly direct-constructible or move-constructible.", [](auto& fixture)
    {
        using ExplicitMoveConstructibleFoo = TupleTestFixture::ExplicitMoveConstructibleFoo;
        using ImplicitMoveConstructibleFoo = TupleTestFixture::ImplicitMoveConstructibleFoo;
        using ImplicitlyConvertibleFoo = TupleTestFixture::ImplicitlyConvertibleFoo;

        // [i] Implicit converting-copy ctor: TupleT<U> t = { TupleT<V>(v) };

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ImplicitlyConvertibleFoo>, TupleT<ImplicitMoveConstructibleFoo>&&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ImplicitlyConvertibleFoo>, TupleT<ImplicitMoveConstructibleFoo>&&>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsImplicitlyConstructible<TupleT<ImplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, TupleT<ExplicitMoveConstructibleFoo, ImplicitMoveConstructibleFoo>&&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConstructible<TupleT<ImplicitlyConvertibleFoo, ImplicitlyConvertibleFoo>, TupleT<ExplicitMoveConstructibleFoo, ImplicitMoveConstructibleFoo>&&>), true);
     })

    .TestCase("A tuple size is equal to the number of elements it can store.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::Rank<TupleT<>>), 0);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::Rank<TupleT<Int>>), 1);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::Rank<TupleT<Int, Float>>), 2);
    })

    .TestCase("Discarding no elements from a tuple returns the tuple unchanged.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<Templates::TuplePopFront<0, TupleT<Int>>, TupleT<Int>>), true);
    })

    .TestCase("Discarding elements from a tuple returns the tuple of the remaining elements.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<Templates::TuplePopFront<1, TupleT<Int, Float, Bool>>, TupleT<Float, Bool>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<Templates::TuplePopFront<2, TupleT<Int, Float, Bool>>, TupleT<Bool>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<Templates::TuplePopFront<3, TupleT<Int, Float, Bool>>, TupleT<>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<Templates::TuplePopFront<1, TupleT<Int>>, TupleT<>>), true);
    })

    .TestCase("TupleElement trait is used to get the type a tuple element by index.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<Templates::TupleElement<0, TupleT<Bool>>, Bool>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<Templates::TupleElement<0, TupleT<Int, Float>>, Int>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<Templates::TupleElement<1, TupleT<Int, Float>>, Float>), true);
    })

    .TestCase("Tuples provide read-access by index to their elements.", [](auto& fixture)
    {
        auto scalar = Int{ 3 };

        auto tuple_a = TupleT<const Int, Int, Int&, Float>{ 100, 200, scalar, 400.0f };
        auto tuple_b = TupleT<const Int, Int, Int&, Float>{ 100, 200, scalar, 400.0f };

        scalar = 300;

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsLValueReference<decltype(Get<0>(tuple_a))>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConst<Syntropy::Templates::RemoveReference<decltype(Get<0>(tuple_a))>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsRValueReference<decltype(Get<1>(Move(tuple_a)))>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsConst<Syntropy::Templates::RemoveReference<decltype(Get<1>(Move(tuple_a)))>>), false);

        SYNTROPY_UNIT_EQUAL((Get<0>(tuple_a)), 100);
        SYNTROPY_UNIT_EQUAL((Get<1>(ReadOnly(tuple_a))), 200);
        SYNTROPY_UNIT_EQUAL((Get<2>(tuple_a)), 300);
        SYNTROPY_UNIT_EQUAL((Get<3>(Move(ReadOnly(tuple_a)))), 400.0f);
        SYNTROPY_UNIT_EQUAL((Get<3>(Move(tuple_a))), 400.0f);
    })

    .TestCase("Tuples provide read-write access by index to their elements.", [](auto& fixture)
    {
        auto scalar = Int{ 3 };

        auto tuple = TupleT<const Int, Int, Int&, Float>{ 1, 2, scalar, 4.0f };

        // Get<0>(tuple) = 100;
        Get<1>(tuple) = 200;
        Get<2>(tuple) = 300;
        Get<3>(tuple) = 400.0f;

        SYNTROPY_UNIT_EQUAL((Get<0>(tuple)), 1);
        SYNTROPY_UNIT_EQUAL((Get<1>(tuple)), 200);
        SYNTROPY_UNIT_EQUAL((Get<2>(tuple)), 300);
        SYNTROPY_UNIT_EQUAL((Get<3>(tuple)), 400.0f);

        SYNTROPY_UNIT_EQUAL(scalar, 300);
    })

    .TestCase("Tuples provide read-access by type to their elements.", [](auto& fixture)
    {
        auto scalar = Int{ 3 };

        auto tuple_a = TupleT<const Int, Int, Int&, Float>{ 100, 200, scalar, 400.0f };
        auto tuple_b = TupleT<const Int, Int, Int&, Float>{ 100, 200, scalar, 400.0f };

        scalar = 300;

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<decltype(Get<const Int>(tuple_a)), const Int&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<decltype(Get<Int>(tuple_a)), Int&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<decltype(Get<Int&>(tuple_a)), Int&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<decltype(Get<Float>(tuple_a)), float&>), true);

        SYNTROPY_UNIT_EQUAL((Get<const Int>(tuple_a)), 100);
        SYNTROPY_UNIT_EQUAL((Get<Int>(ReadOnly(tuple_a))), 200);
        SYNTROPY_UNIT_EQUAL((Get<Int&>(tuple_a)), 300);
        SYNTROPY_UNIT_EQUAL((Get<Float>(Move(ReadOnly(tuple_a)))), 400.0f);
        SYNTROPY_UNIT_EQUAL((Get<Float>(Move(tuple_a))), 400.0f);
    })

    .TestCase("Tuples provide read-write access by type to their elements.", [](auto& fixture)
    {
        auto scalar = Int{ 3 };

        auto tuple = TupleT<const Int, Int, Int&, Float>{ 1, 2, scalar, 4.0f };

        // Get<0>(tuple) = 100;
        Get<1>(tuple) = 200;
        Get<2>(tuple) = 300;
        Get<3>(tuple) = 400.0f;

        SYNTROPY_UNIT_EQUAL((Get<const Int>(tuple)), 1);
        SYNTROPY_UNIT_EQUAL((Get<Int>(tuple)), 200);
        SYNTROPY_UNIT_EQUAL((Get<Int&>(tuple)), 300);
        SYNTROPY_UNIT_EQUAL((Get<Float>(tuple)), 400.0f);

        SYNTROPY_UNIT_EQUAL(scalar, 300);
    })

    .TestCase("Tuples with the same elements are equal.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((TupleT<Int, Float>{ 10, 20.0f } == TupleT<Int, Float>{ 10, 20.0f }), true);
        SYNTROPY_UNIT_EQUAL((TupleT<Int, Float>{ 10, 20.0f } != TupleT<Int, Float>{ 20, 10.0f }), true);
    })

    .TestCase("Tuples whose elements compare equivalent are equal, even if they have different types.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((TupleT<Int, Float>{ 10, 20.0f } == TupleT<Float, Int>{ 10.0f, 20 }), true);
    })

    .TestCase("Copy-constructed tuples are equal to each other.", [](auto& fixture)
    {
        auto tuple_source = TupleT<Int, Float>{ 10, 20.0f };
        auto tuple_copy = tuple_source;

        SYNTROPY_UNIT_EQUAL(tuple_copy == tuple_source, true);
    })
    
    .TestCase("Empty tuples are trivially copy-assignable.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsCopyAssignable<TupleT<>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsTriviallyCopyAssignable<TupleT<>>), true);
    })
    
    .TestCase("Empty tuples are trivially move-assignable.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsMoveAssignable<TupleT<>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsTriviallyMoveAssignable<TupleT<>>), true);
    })

    .TestCase("Tuples with copy-assignable elements are copy-assignable themselves.", [](auto& fixture)
    {
        using MovableOnlyFoo = TupleTestFixture::MovableOnlyFoo;
        using CopyableOnlyFoo = TupleTestFixture::CopyableOnlyFoo;

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsCopyAssignable<TupleT<Int, Float>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsCopyAssignable<TupleT<Int, Float, CopyableOnlyFoo>>), true);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsCopyAssignable<TupleT<Int, Float, MovableOnlyFoo >>), false);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsCopyAssignable<TupleT<Int, Float, MovableOnlyFoo, CopyableOnlyFoo >>), false);
    })
        
    .TestCase("Tuples with move-assignable elements are move-assignable themselves.", [](auto& fixture)
    {
        using MovableOnlyFoo = TupleTestFixture::MovableOnlyFoo;
        using CopyableOnlyFoo = TupleTestFixture::CopyableOnlyFoo;

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsMoveAssignable<TupleT<Int, Float>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsMoveAssignable<TupleT<Int, Float, MovableOnlyFoo>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsMoveAssignable<TupleT<Int, Float, CopyableOnlyFoo>>), true);                     // Will fallback on the copy constructor.
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsMoveAssignable<TupleT<Int, Float, MovableOnlyFoo, CopyableOnlyFoo >>), true);    // Will fallback on the copy constructor.
    })

    .TestCase("Copy-constructing and copy-assigning a tuple the same values produces two equal tuples.", [](auto& fixture)
    {
        auto copy_construct_tuple = TupleT<Int, Float>{ 10, 30.0f };

        auto copy_assign_tuple = TupleT<Int, Float>{};

        copy_assign_tuple = copy_construct_tuple;

        SYNTROPY_UNIT_EQUAL(copy_construct_tuple == copy_assign_tuple, true);
    })

    .TestCase("Elements are moved from a tuple to another by means of a move-constructor.", [](auto& fixture)
    {
        using TestMovableOnlyFoo = TupleTestFixture::TestMovableOnlyFoo;

        auto source_tuple = TupleT<TestMovableOnlyFoo>{};
        auto destination_tuple = TupleT<TestMovableOnlyFoo>{ Move(source_tuple) };       // @rfacendola Accessing source_tuple after this point results in undefined behavior.

        SYNTROPY_UNIT_EQUAL((Syntropy::Get<0>(source_tuple).moved_), true);
    })
        
    .TestCase("Tuples can implicitly convert elements during copy assignment.", [](auto& fixture)
    {
        using MovableOnlyFoo = TupleTestFixture::MovableOnlyFoo;

        auto tuple_int = TupleT<Int>{ 10 };
        auto tuple_float = TupleT<Float>{ 20.0f };

        // tuple_float = tuple_int;

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsAssignable<TupleT<Int>&, const TupleT<Float>&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsAssignable<TupleT<Float>&, const TupleT<Int>&>), true);
    })

    .TestCase("Tuples can implicitly convert elements during move assignment.", [](auto& fixture)
    {
        using TestMovableOnlyFoo = TupleTestFixture::TestMovableOnlyFoo;
        using TestMovableOnlyBar = TupleTestFixture::TestMovableOnlyBar;

        auto tuple_foo = TupleT<TestMovableOnlyFoo>{};
        auto tuple_bar = TupleT<TestMovableOnlyBar>{};

        tuple_bar = Move(tuple_foo);

        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsAssignable<TupleT<TestMovableOnlyBar>&, TupleT<TestMovableOnlyFoo>&&>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsAssignable<TupleT<TestMovableOnlyFoo>&, TupleT<TestMovableOnlyBar>&&>), false);

        SYNTROPY_UNIT_EQUAL((Syntropy::Get<0>(tuple_foo).moved_), true);
    })

    .TestCase("Swapping an empty tuple with another empty tuple has no effect.", [](auto& fixture)
    {
        auto lhs = MakeTuple();
        auto rhs = MakeTuple();

        Syntropy::Swap(lhs, rhs);

        SYNTROPY_UNIT_EQUAL(lhs == rhs, true);
        SYNTROPY_UNIT_EQUAL(lhs == MakeTuple(), true);
        SYNTROPY_UNIT_EQUAL(rhs == MakeTuple(), true);
    })

    .TestCase("Swapping an two tuples swaps each element in the first with the corresponding element in the latter.", [](auto& fixture)
    {
        auto lhs = MakeTuple(10, 20.0f);
        auto rhs = MakeTuple(30, 40.0f);

        Syntropy::Swap(lhs, rhs);

        SYNTROPY_UNIT_EQUAL((Syntropy::Get<0>(lhs)), 30);
        SYNTROPY_UNIT_EQUAL((Syntropy::Get<1>(lhs)), 40.0f);
        SYNTROPY_UNIT_EQUAL((Syntropy::Get<0>(rhs)), 10);
        SYNTROPY_UNIT_EQUAL((Syntropy::Get<1>(rhs)), 20);
    })

    .TestCase("Modifying an elment of a tuple created by means of ::Tie, reflects on the original argument.", [](auto& fixture)
    {

        auto element_a = 10;
        auto element_b = 20;

        auto tuple = Syntropy::Tie(element_a, element_b);

        SYNTROPY_UNIT_EQUAL((Syntropy::Get<0>(tuple)), 10);
        SYNTROPY_UNIT_EQUAL((Syntropy::Get<1>(tuple)), 20);

        Syntropy::Get<0>(tuple) = 100;

        SYNTROPY_UNIT_EQUAL((Syntropy::Get<0>(tuple)), 100);
        SYNTROPY_UNIT_EQUAL((Syntropy::Get<1>(tuple)), 20);
    })

    .TestCase("When forwarding-as-tuple, all elements get perfectly-forwarded.", [](auto& fixture)
    {
        using TestMovableOnlyFoo = TupleTestFixture::TestMovableOnlyFoo;

        auto movable_bar = TestMovableOnlyFoo{};
        auto reference_foo = 100.0f;

        auto tuple = Syntropy::ForwardAsTuple(Move(movable_bar), reference_foo);

        auto movable_foo = Move(Syntropy::Get<0>(tuple));           // Preserve rvalues.
        Syntropy::Get<1>(tuple) = 10.0f;                            // Preserve lvalues.

        SYNTROPY_UNIT_EQUAL(reference_foo, 10.0f);
        SYNTROPY_UNIT_EQUAL(movable_bar.moved_, true);
    });

}