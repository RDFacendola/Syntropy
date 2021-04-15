
/// \file ut_concepts.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/core/foundation/ordering.h"

#include "syntropy/diagnostics/unit_test/unit_test.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* CONCEPTS TEST FIXTURE                                                */
    /************************************************************************/

    /// \brief Concepts test fixture.
    struct ConceptsTestFixture
    {
        template <typename T>
        struct Template{};

        template <typename T>
        struct Template2{};
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& ut_concepts
        = Syntropy::UnitTest::MakeAutoUnitTest<ConceptsTestFixture>(
            u8"concepts.templates.language.syntropy")

    .TestCase(u8"Types are same-as themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsSame<Int, Int>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsSame<Int, Int, Int>), true);
    })

    .TestCase(u8"Types are not same-as other types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsSame<Int, Float>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsSame<Int, Float, Int>),false);
    })

    .TestCase(u8"Reference-types are not same-as their value-types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsSame<Int, Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Movable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Immovable<Int>>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsSame<Int, Mutable<Int>, Int>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Immutable<Int>, Int>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Movable<Int>, Int>), false);
        SYNTROPY_UNIT_EQUAL((IsSame<Int, Immovable<Int>, Int>), false);
    })

    .TestCase(u8"Value-types are not same-as their reference-types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsSame<Mutable<Int>, Int>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsSame<Immutable<Int>, Int>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsSame<Movable<Int>, Int>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsSame<Immovable<Int>, Int>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsSame<Mutable<Int>, Int, Mutable<Int>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsSame<Immutable<Int>, Int, Immutable<Int>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsSame<Movable<Int>, Int, Movable<Int>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsSame<Immovable<Int>, Int, Immovable<Int>>), false);
    })

    .TestCase(u8"Types are convertible to themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Int>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Int, Int>), true);
    })

    .TestCase(u8"Types are not convertible to other non-related types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsConvertible<Bool, Ptr<Bool>>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsConvertible<Float, Float, Ptr<Bool>>), false);
    })

    .TestCase(u8"Value-types are convertible to their respective immutable "
              u8"reference-types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Immutable<Int>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Int, Immutable<Int>>), true);
    })

    .TestCase(u8"Value-types are not convertible to their respective mutable "
              u8"reference-types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Mutable<Int>>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Int, Mutable<Int>>), false);
    })

    .TestCase(u8"Value-types are convertible to their respective movable "
              u8"reference-types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Movable<Int>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Int, Movable<Int>>), true);
    })

    .TestCase(u8"Value-types are convertible to their respective immovable "
              u8"reference-types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Immovable<Int>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsConvertible<Int, Int, Immovable<Int>>), true);
    })

    .TestCase(u8"Reference-types are convertible to their respective "
              u8"value-types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Mutable<Int>, Int>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immutable<Int>, Int>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Movable<Int>, Int>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immovable<Int>, Int>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Mutable<Int>, Mutable<Int>, Int>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immutable<Int>, Immutable<Int>, Int>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Movable<Int>, Movable<Int>, Int>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immovable<Int>, Immovable<Int>, Int>), true);
    })

    .TestCase(u8"Reference-types are convertible to their respective "
              u8"immutable reference-types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Mutable<Int>, Immutable<Int>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immutable<Int>, Immutable<Int>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Movable<Int>, Immutable<Int>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immovable<Int>, Immutable<Int>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Mutable<Int>, Mutable<Int>, Immutable<Int>>),
            true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immutable<Int>, Immutable<Int>, Immutable<Int>>),
            true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Movable<Int>, Movable<Int>,Immutable<Int>>),
            true);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immovable<Int>, Immovable<Int>,Immutable<Int>>),
            true);
    })

    .TestCase(u8"Immutable reference-types are not convertible to their "
              u8"respective mutable reference-types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immutable<Int>, Mutable<Int>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immovable<Int>, Movable<Int>>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immutable<Int>, Immutable<Int>, Mutable<Int>>),
            false);

        SYNTROPY_UNIT_EQUAL(
            (IsConvertible<Immovable<Int>, Immovable<Int>, Movable<Int>>),
            false);
    })

    .TestCase(u8"Immutable reference-types are immutable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsImmutable<Immutable<Int>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsImmutable<Immutable<Int>, Immutable<Int>>), true);
    })

    .TestCase(u8"Reference-types other than immutable reference-types are not "
              u8"immutable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsImmutable<Mutable<Int>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsImmutable<Movable<Int>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsImmutable<Immovable<Int>>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsImmutable<Immutable<Int>, Mutable<Int>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsImmutable<Immutable<Int>, Movable<Int>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsImmutable<Immutable<Int>, Immovable<Int>>), false);
    })

    .TestCase(u8"Mutable reference-types are mutable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsMutable<Mutable<Int>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsMutable<Mutable<Int>, Mutable<Int>>), true);
    })

    .TestCase(u8"Reference-types other than mutable reference-types are not "
              u8"mutable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsMutable<Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMutable<Movable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMutable<Immovable<Int>>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsMutable<Mutable<Int>, Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMutable<Mutable<Int>, Movable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMutable<Mutable<Int>, Immovable<Int>>), false);
    })

    .TestCase(u8"Movable reference-types are movable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsMovable<Movable<Int>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsMovable<Movable<Int>, Movable<Int>>), true);
    })

    .TestCase(u8"Reference-types other than movable reference-types are not "
              u8"movable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsMovable<Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMovable<Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMovable<Immovable<Int>>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsMovable<Movable<Int>, Mutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMovable<Movable<Int>, Immutable<Int>>), false);
        SYNTROPY_UNIT_EQUAL((IsMovable<Movable<Int>, Immovable<Int>>), false);
    })

    .TestCase(u8"Immovable reference-types are immovable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsImmovable<Immovable<Int>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsImmovable<Immovable<Int>, Immovable<Int>>), true);
    })

    .TestCase(u8"Reference types other than immovable reference-types are not "
              u8"immovable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsImmovable<Mutable<Int>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsImmovable<Immutable<Int>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsImmovable<Movable<Int>>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsImmovable<Immovable<Int>, Mutable<Int>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsImmovable<Immovable<Int>, Immutable<Int>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsImmovable<Immovable<Int>, Movable<Int>>), false);
    })

    .TestCase(u8"Bool types are boolean.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsBoolean<Bool>), true);
        SYNTROPY_UNIT_EQUAL((IsBoolean<Mutable<Bool>>), true);
        SYNTROPY_UNIT_EQUAL((IsBoolean<Immutable<Bool>>), true);
        SYNTROPY_UNIT_EQUAL((IsBoolean<Movable<Bool>>), true);
        SYNTROPY_UNIT_EQUAL((IsBoolean<Immovable<Bool>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsBoolean<Bool,
                                       Mutable<Bool>,
                                       Immutable<Bool>,
                                       Movable<Bool>,
                                       Immovable<Bool>>), true);
    })

    .TestCase(u8"Non-bool types are not boolean.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsBoolean<Int>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsBoolean<Bool, Int>), false);
    })

    .TestCase(u8"Integral types are integral.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsIntegral<Int>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Mutable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Immutable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Movable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Immovable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Fix8>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Fix16>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Fix32>), true);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Fix64>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsIntegral<Int,
                                        Mutable<Int>,
                                        Immutable<Int>,
                                        Movable<Int>,
                                        Immovable<Int>,
                                        Fix8,
                                        Fix16,
                                        Fix32,
                                        Fix64>), true);

    })

    .TestCase(u8"Non-integral types are not integral.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsIntegral<Bool>), false);
        SYNTROPY_UNIT_EQUAL((IsIntegral<Float>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsIntegral<Int, Bool>), false);
    })

    .TestCase(u8"Float types are real.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsReal<Float>), true);
        SYNTROPY_UNIT_EQUAL((IsReal<Mutable<Float>>), true);
        SYNTROPY_UNIT_EQUAL((IsReal<Immutable<Float>>), true);
        SYNTROPY_UNIT_EQUAL((IsReal<Movable<Float>>), true);
        SYNTROPY_UNIT_EQUAL((IsReal<Immovable<Float>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsReal<Float,
                                    Mutable<Float>,
                                    Immutable<Float>,
                                    Movable<Float>,
                                    Immovable<Float>>), true);
    })

    .TestCase(u8"Non-float types are not real.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsReal<Int>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsReal<Float, Bool>), false);
    })

    .TestCase(u8"Classes are derived from all their direct bases.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct BaseA{};
        struct BaseB{};
        struct Derived : BaseA, BaseB{};

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsDerivedFrom<Derived, BaseA>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsDerivedFrom<Derived, BaseB>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsDerivedFrom<Derived, BaseA, BaseB>), true);

        // Reference-types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsDerivedFrom<Immutable<Derived>, BaseA>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsDerivedFrom<Derived, Immutable<BaseB>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsDerivedFrom<Derived, Mutable<BaseA>, BaseB>), true);
    })

    .TestCase(u8"Classes are derived from all their indirect bases.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct BaseA{};
        struct BaseB{};
        struct IntermediateA : BaseA{};
        struct IntermediateB : BaseB{};
        struct Derived : IntermediateA, IntermediateB{};

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, BaseA>), true);
        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, BaseB>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, BaseA, BaseB>), true);
    })

    .TestCase(u8"Classes are not derived from all other unrelated classes.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Base{};
        struct Derived : Base{};
        struct Unrelated{};

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Base, Derived>), false);
        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, Unrelated>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsDerivedFrom<Derived, Base, Unrelated>), false);
    })

    .TestCase(u8"Classes are base of all their direct derived classes.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Base{};
        struct DerivedA : Base{};
        struct DerivedB : Base{};

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsBaseOf<Base, DerivedA>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsBaseOf<Base, DerivedB>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsBaseOf<Base, DerivedA, DerivedB>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsBaseOf<Immutable<Base>, DerivedA>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsBaseOf<Base, Immutable<DerivedB>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsBaseOf<Base, Mutable<DerivedA>, DerivedB>), true);
    })

    .TestCase(u8"Classes are base of all their indirect derived classes.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Base{};
        struct Intermediate : Base{};
        struct DerivedA : Intermediate{};
        struct DerivedB : Intermediate{};

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, DerivedA>), true);
        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, DerivedB>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, DerivedA, DerivedB>), true);
    })

    .TestCase(u8"Classes are not base of other unrelated classes.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Base{};
        struct Derived : Base{};
        struct Unrelated{};

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, Unrelated>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsBaseOf<Base, Derived, Unrelated>), false);
    })

    .TestCase(u8"Virtual classes are polymorphic.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct PolymorphicA
        {
            virtual ~PolymorphicA() = default;
        };

        struct PolymorphicB
        {
            virtual ~PolymorphicB() = default;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsPolymorphic<PolymorphicA>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsPolymorphic<PolymorphicB>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsPolymorphic<PolymorphicA, PolymorphicB>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsPolymorphic<Immutable<PolymorphicA>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsPolymorphic<Immutable<PolymorphicB>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsPolymorphic<Immutable<PolymorphicA>, Immutable<PolymorphicB>>),
            true);
    })

    .TestCase(u8"Non-virtual classes are not polymorphic.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Base{};
        struct Derived : Base{};

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsPolymorphic<Base>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsPolymorphic<Derived>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsPolymorphic<Base, Derived>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsPolymorphic<Immutable<Base>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsPolymorphic<Immutable<Derived>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsPolymorphic<Immutable<Base>, Immutable<Derived>>), false);
    })

    .TestCase(u8"Final classes are final.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct FinalA final{};
        struct FinalB final{};

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsFinal<FinalA>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsFinal<FinalA, FinalB>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsFinal<Immutable<FinalA>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsFinal<Immutable<FinalA>, Immutable<FinalB>>), true);
    })

    .TestCase(u8"Non-final classes are not final.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonFinalA {};
        struct NonFinalB {};

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsFinal<NonFinalA>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsFinal<NonFinalA, NonFinalB>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsFinal<Immutable<NonFinalA>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsFinal<Immutable<NonFinalA>, Immutable<NonFinalB>>), false);
    })

    .TestCase(u8"Fundamental types are standard-layout types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Bool>), true);
        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Int>), true);
        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Float>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Bool, Int, Float>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Immutable<Bool>>), true);
        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Movable<Int>>), true);
        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Immovable<Float>>), true);
    })

    .TestCase(u8"POD types are standard layout types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct PODA
        {
            Int Foo;
        };

        struct PODB
        {
            Int Foo;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<PODA>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<PODA, PODB>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Immutable<PODA>>), true);
    })

    .TestCase(u8"Polymorphic types are not standard layout types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct PolymorphicA
        {
            virtual ~PolymorphicA() = default;
        };

        struct PolymorphicB
        {
            virtual ~PolymorphicB() = default;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsStandardLayoutType<PolymorphicA>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsStandardLayoutType<PolymorphicA, PolymorphicB>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsStandardLayoutType<Immutable<PolymorphicA>>), false);
    })

    .TestCase(u8"Types with different access control are not standard layout "
              u8"types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct AccessControl{ public: Int foo_; private: Int bar_; };

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<AccessControl>),
                            false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Int, AccessControl>),
                            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Immutable<AccessControl>>),
                            false);
    })

    .TestCase(u8"Polymorphic types are not standard layout types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Polymorphic{ virtual ~Polymorphic() = default; };

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Polymorphic>),
                            false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Int, Polymorphic>),
                            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsStandardLayoutType<Immutable<Polymorphic>>),
                            false);
    })

    .TestCase(u8"Default constructible types are constructible from an empty "
              u8"list of arguments",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct DefaultConstructibleA{};
        struct DefaultConstructibleB{};

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsConstructibleFrom<DefaultConstructibleA>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsConstructibleFrom<Immutable<DefaultConstructibleA>>), true);
    })

    .TestCase(u8"Fundamental types are constructible from an empty list of "
              u8"arguments.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConstructibleFrom<Bool>), true);
        SYNTROPY_UNIT_EQUAL((IsConstructibleFrom<Int>), true);
        SYNTROPY_UNIT_EQUAL((IsConstructibleFrom<Float>), true);
    })

    .TestCase(u8"Types are constructible from their constructor arguments.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Constructible
        {
            Constructible(Int, Float){}
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsConstructibleFrom<Constructible, Int, Float>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsConstructibleFrom<Immutable<Constructible>, Int, Float>), true);
    })

    .TestCase(u8"Types are not constructible from arguments that match no "
              u8"constructor.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Constructible
        {
            Constructible(Int, Float){}
        };

        SYNTROPY_UNIT_EQUAL(
            (IsConstructibleFrom<Constructible, Float>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsConstructibleFrom<Constructible, Float, Int, Int>), false);
    })

    .TestCase(u8"Default constructible types are default constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct DefaultConstructibleA{};
        struct DefaultConstructibleB{};

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsDefaultConstructible<DefaultConstructibleA>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsDefaultConstructible<DefaultConstructibleA,
                                    DefaultConstructibleB>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsDefaultConstructible<Immutable<DefaultConstructibleA>>), true);
    })

    .TestCase(u8"Non-default constructible types are not default "
              u8"constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonDefaultConstructible{ NonDefaultConstructible(Int){} };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsDefaultConstructible<NonDefaultConstructible>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsDefaultConstructible<Int, NonDefaultConstructible>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsDefaultConstructible<Immutable<NonDefaultConstructible>>),
            false);
    })

    .TestCase(u8"Fundamental types are default constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsDefaultConstructible<Bool>), true);
        SYNTROPY_UNIT_EQUAL((IsDefaultConstructible<Int>), true);
        SYNTROPY_UNIT_EQUAL((IsDefaultConstructible<Float>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsDefaultConstructible<Bool, Int, Float>), true);
    })

    .TestCase(u8"Default constructible types are default initializable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct DefaultConstructible{};

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsDefaultInitializable<DefaultConstructible>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsDefaultInitializable<Int, DefaultConstructible>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsDefaultInitializable<Immutable<DefaultConstructible>>), true);
    })

    .TestCase(u8"Fundamental types are default initializable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsDefaultInitializable<Bool>), true);
        SYNTROPY_UNIT_EQUAL((IsDefaultInitializable<Int>), true);
        SYNTROPY_UNIT_EQUAL((IsDefaultInitializable<Float>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsDefaultInitializable<Bool, Int, Float>), true);
    })

    .TestCase(u8"Copy constructible types are copy-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct CopyConstructible
        {
            CopyConstructible(Immutable<CopyConstructible>){}
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsCopyConstructible<CopyConstructible>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsCopyConstructible<Int, CopyConstructible>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsCopyConstructible<Immutable<CopyConstructible>>), true);
    })

    .TestCase(u8"Non-copy constructible types are not copy-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonCopyConstructible
        {
            NonCopyConstructible(Immutable<NonCopyConstructible>) = delete;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsCopyConstructible<NonCopyConstructible>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsCopyConstructible<Int, NonCopyConstructible>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsCopyConstructible<Immutable<NonCopyConstructible>>), false);
    })

    .TestCase(u8"Move constructible types are move-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct MoveConstructible
        {
            MoveConstructible(Movable<MoveConstructible>){}
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsMoveConstructible<MoveConstructible>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsMoveConstructible<Int, MoveConstructible>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsMoveConstructible<Immutable<MoveConstructible>>), true);
    })

    .TestCase(u8"Non-move constructible types are not move-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonMoveConstructible
        {
            NonMoveConstructible(Immutable<NonMoveConstructible>) = delete;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsMoveConstructible<NonMoveConstructible>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsMoveConstructible<Int, NonMoveConstructible>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsMoveConstructible<Immutable<NonMoveConstructible>>), false);
    })

    .TestCase(u8"Copy-assignable types are copy-assignable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct CopyAssignable
        {
            Mutable<CopyAssignable>
            operator=(Immutable<CopyAssignable>)
            {
                return *this;
            }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsCopyAssignable<CopyAssignable>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsCopyAssignable<Int, CopyAssignable>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsCopyAssignable<Immutable<CopyAssignable>>), true);
    })

    .TestCase(u8"Non-copy-assignable types are not copy-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonCopyAssignable
        {
            Mutable<NonCopyAssignable>
            operator=(Immutable<NonCopyAssignable>) = delete;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsCopyAssignable<NonCopyAssignable>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsCopyAssignable<Int, NonCopyAssignable>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsCopyAssignable<Immutable<NonCopyAssignable>>), false);
    })

    .TestCase(u8"Move-assignable types are move-assignable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct MoveAssignable
        {
            // Movable<X> apparently stimulates some clang bug.

            Mutable<MoveAssignable>
            operator=(MoveAssignable&&)
            {
                return *this;
            }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsMoveAssignable<MoveAssignable>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsMoveAssignable<Int, MoveAssignable>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsMoveAssignable<Immutable<MoveAssignable>>), true);
    })

    .TestCase(u8"Non-move-assignable types are not move-assignable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonMoveAssignable
        {
            // Movable<X> apparently stimulates some clang bug.

            Mutable<NonMoveAssignable>
            operator=(NonMoveAssignable&&) = delete;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsMoveAssignable<NonMoveAssignable>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsMoveAssignable<Int, NonMoveAssignable>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsMoveAssignable<Immutable<NonMoveAssignable>>), false);
    })

    .TestCase(u8"Copy-assignable types can be assignable from themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct CopyAssignable
        {
            Mutable<CopyAssignable>
            operator=(Immutable<CopyAssignable>){ return *this; }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<CopyAssignable, CopyAssignable>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<CopyAssignable, Immutable<CopyAssignable>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<CopyAssignable, Mutable<CopyAssignable>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<CopyAssignable,
                              CopyAssignable,
                              Immutable<CopyAssignable>,
                              Mutable<CopyAssignable>>),
            true);
    })

    .TestCase(u8"Move-assignable types can be assigned from themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct MoveAssignable
        {
            // Movable<X> apparently stimulates some clang bug.

            Mutable<MoveAssignable>
            operator=(MoveAssignable&&){ return *this; }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<MoveAssignable, MoveAssignable>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<MoveAssignable, Movable<MoveAssignable>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<MoveAssignable,
                              MoveAssignable,
                              Movable<MoveAssignable>>),
            true);
    })

    .TestCase(u8"Types cannot be assigned from unrelated types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct SomeType{};
        struct UnrelatedType{};

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<SomeType, UnrelatedType>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<SomeType, Immutable<UnrelatedType>>), false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<SomeType, Movable<UnrelatedType>>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<SomeType, SomeType, UnrelatedType>), false);
    })

    .TestCase(u8"Types can be assigned from types that are implicitly "
              u8"convertible to them.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Convertible{};
        struct SomeType
        {
            SomeType(Immutable<Convertible>){}
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<SomeType, Convertible>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<SomeType, Immutable<Convertible>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<SomeType, Movable<Convertible>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<SomeType,
                              Convertible,
                              Immutable<Convertible>,
                              Movable<Convertible>>),
            true);

    })

    .TestCase(u8"Immutable reference-types are not assignable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct CopyAssignable
        {
            Mutable<CopyAssignable>
            operator=(Immutable<CopyAssignable>){ return *this; }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<Immutable<CopyAssignable>,
                              CopyAssignable>),
            false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<Immutable<CopyAssignable>,
                              Immutable<CopyAssignable>>),
            false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableFrom<Immutable<CopyAssignable>,
                              Mutable<CopyAssignable>>),
            false);

    })

    .TestCase(u8"Copy-assignable types can be assigned to themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct CopyAssignable
        {
            Mutable<CopyAssignable>
            operator=(Immutable<CopyAssignable>){ return *this; }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<CopyAssignable, CopyAssignable>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Immutable<CopyAssignable>, CopyAssignable>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Mutable<CopyAssignable>, CopyAssignable>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<CopyAssignable, Mutable<CopyAssignable>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Immutable<CopyAssignable>,
                            Mutable<CopyAssignable>>),
            true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Mutable<CopyAssignable>,
                            Mutable<CopyAssignable>>),
            true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<CopyAssignable,
                            CopyAssignable,
                            Mutable<CopyAssignable>>),
            true);
    })

    .TestCase(u8"Move-assignable types can be assigned to themselves.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct MoveAssignable
        {
            // Movable<X> apparently stimulates some clang bug.

            Mutable<MoveAssignable>
            operator=(MoveAssignable&&)
            {
                return *this;
            }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<MoveAssignable, MoveAssignable>),
            true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Movable<MoveAssignable>, MoveAssignable>),
            true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<MoveAssignable, Mutable<MoveAssignable>>),
            true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Movable<MoveAssignable>, Mutable<MoveAssignable>>),
            true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<MoveAssignable,
                            MoveAssignable,
                            Mutable<MoveAssignable>>),
            true);
    })

    .TestCase(u8"Types cannot be assigned from unrelated types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct SomeType{};
        struct UnrelatedType{};

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<UnrelatedType, SomeType>),
            false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Immutable<UnrelatedType>, SomeType>),
            false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Mutable<UnrelatedType>, SomeType>),
            false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<UnrelatedType, Mutable<SomeType>>),
            false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Immutable<UnrelatedType>, Mutable<SomeType>>),
            false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Mutable<UnrelatedType>, Mutable<SomeType>>),
            false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<UnrelatedType, UnrelatedType, SomeType>),
            false);
    })

    .TestCase(u8"Types can be assigned from types that are implicitly "
              u8"convertible to them.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Convertible{};
        struct SomeType{ SomeType(Immutable<Convertible>){} };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Convertible, SomeType>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Immutable<Convertible>, SomeType>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Mutable<Convertible>, SomeType>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Convertible, Mutable<SomeType>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Immutable<Convertible>, Mutable<SomeType>>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Mutable<Convertible>, Mutable<SomeType>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Convertible, SomeType, Mutable<SomeType>>), true);
    })

    .TestCase(u8"Nothing can be assigned to immutable reference-types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct CopyAssignable
        {
            Mutable<CopyAssignable>
            operator=(Immutable<CopyAssignable>)
            {
                return *this;
            }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<CopyAssignable, Immutable<CopyAssignable>>),
            false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Immutable<CopyAssignable>,
                            Immutable<CopyAssignable>>),
            false);

        SYNTROPY_UNIT_EQUAL(
            (IsAssignableTo<Mutable<CopyAssignable>,
                            Immutable<CopyAssignable>>),
            false);
    })

    .TestCase(u8"Types with public destructor are destructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Destructible
        {
            ~Destructible() = default;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsDestructible<Destructible>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsDestructible<Int, Destructible>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsDestructible<Immutable<Destructible>>), true);
    })

    .TestCase(u8"Fundamental types are destructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsDestructible<Bool>), true);
        SYNTROPY_UNIT_EQUAL((IsDestructible<Int>), true);
        SYNTROPY_UNIT_EQUAL((IsDestructible<Float>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsDestructible<Bool, Int, Float>), true);
    })

    .TestCase(u8"Types with private destructor are not destructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonDestructible
        {
        private:
            ~NonDestructible() = default;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsDestructible<NonDestructible>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsDestructible<Bool, NonDestructible>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsDestructible<Immutable<NonDestructible>>), false);
    })

    .TestCase(u8"Trivial default constructible types are trivially default "
              u8"constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct TriviallyDefaultConstructible{};

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDefaultConstructible<TriviallyDefaultConstructible>),
            true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDefaultConstructible<Int,
                                             TriviallyDefaultConstructible>),
            true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDefaultConstructible<
                Immutable<TriviallyDefaultConstructible>>),
            true);
    })

    .TestCase(u8"Non-trivial-default constructible types are not trivially "
              u8"default constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonTriviallyDefaultConstructible
        {
            NonTriviallyDefaultConstructible(){}
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDefaultConstructible<
                NonTriviallyDefaultConstructible>),
            false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDefaultConstructible<
                Int,
                NonTriviallyDefaultConstructible>),
            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDefaultConstructible<
                Immutable<NonTriviallyDefaultConstructible>>),
            false);
    })

    .TestCase(u8"Fundamental types are trivially default constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDefaultConstructible<Bool>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDefaultConstructible<Int>), true);

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDefaultConstructible<Float>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDefaultConstructible<Bool, Int, Float>), true);
    })

    .TestCase(u8"Trivial copy constructible types are "
              u8"trivially-copy-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct TriviallyCopyConstructible
        {
            TriviallyCopyConstructible(
                Immutable<TriviallyCopyConstructible>) = default;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyConstructible<TriviallyCopyConstructible>),
            true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyConstructible<Int, TriviallyCopyConstructible>),
            true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyConstructible<
                Immutable<TriviallyCopyConstructible>>),
            true);
    })

    .TestCase(u8"Non-trivial copy constructible types are not "
              u8"trivially-copy-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonTriviallyCopyConstructible
        {
            NonTriviallyCopyConstructible(
                Immutable<NonTriviallyCopyConstructible>){};
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyConstructible<NonTriviallyCopyConstructible>),
            false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyConstructible<Int, NonTriviallyCopyConstructible>),
            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyConstructible<
                Immutable<NonTriviallyCopyConstructible>>),
            false);
    })

    .TestCase(u8"Trivial move constructible types are "
              u8"trivially-move-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct TriviallyMoveConstructible
        {
             TriviallyMoveConstructible(TriviallyMoveConstructible&&)
                = default;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyMoveConstructible<TriviallyMoveConstructible>),
            true);

        // Varidic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyMoveConstructible<Int, TriviallyMoveConstructible>),
            true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyMoveConstructible<
                Immutable<TriviallyMoveConstructible>>),
            true);
    })

    .TestCase(u8"Non-trivial move constructible types are not "
              u8"trivially-move-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonTriviallyMoveConstructible
        {
            NonTriviallyMoveConstructible(
                Immutable<NonTriviallyMoveConstructible>){}
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyMoveConstructible<NonTriviallyMoveConstructible>),
            false);

        // Varidic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyMoveConstructible<Int, NonTriviallyMoveConstructible>),
            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyMoveConstructible<
                Immutable<NonTriviallyMoveConstructible>>),
            false);
    })

    .TestCase(u8"Trivial-copy-assignable types are trivially-copy-assignable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct TriviallyCopyAssignable
        {
            Mutable<TriviallyCopyAssignable>
            operator=(Immutable<TriviallyCopyAssignable>) = default;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyAssignable<TriviallyCopyAssignable>),
            true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyAssignable<Int, TriviallyCopyAssignable>),
            true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyAssignable<Immutable<TriviallyCopyAssignable>>),
            true);
    })

    .TestCase(u8"Non-copy-assignable types are not copy-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonTriviallyCopyAssignable
        {
            Mutable<NonTriviallyCopyAssignable>
            operator=(Immutable<NonTriviallyCopyAssignable>)
            {
                return *this;
            };
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyAssignable<NonTriviallyCopyAssignable>),
            false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyAssignable<Int, NonTriviallyCopyAssignable>),
            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyAssignable<Immutable<NonTriviallyCopyAssignable>>),
            false);
    })

    .TestCase(u8"Trivial-move-assignable types are trivially-move-assignable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct TriviallyMoveAssignable
        {
            Mutable<TriviallyMoveAssignable>
            operator=(TriviallyMoveAssignable&&) = default;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyMoveAssignable<TriviallyMoveAssignable>),
            true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyMoveAssignable<Int, TriviallyMoveAssignable>),
            true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyMoveAssignable<Immutable<TriviallyMoveAssignable>>),
            true);
    })

    .TestCase(u8"Non-move-assignable types are not move-constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonTriviallyMoveAssignable
        {
            Mutable<NonTriviallyMoveAssignable>
            operator=(NonTriviallyMoveAssignable&&){ return *this; };
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyMoveAssignable<NonTriviallyMoveAssignable>),
            false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyMoveAssignable<Int, NonTriviallyMoveAssignable>),
            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyMoveAssignable<Immutable<NonTriviallyMoveAssignable>>),
            false);
    })

    .TestCase(u8"Trivial destructible types are trivially-destructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct TriviallyDestructible
        {
            ~TriviallyDestructible() = default;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDestructible<TriviallyDestructible>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDestructible<Int, TriviallyDestructible>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDestructible<Immutable<TriviallyDestructible>>), true);
    })

    .TestCase(u8"Non trivial destructible types are "
              u8"non-trivially-destructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonTriviallyDestructible
        {
            ~NonTriviallyDestructible() {};
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDestructible<NonTriviallyDestructible>),
            false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDestructible<Int, NonTriviallyDestructible>),
            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyDestructible<Immutable<NonTriviallyDestructible>>),
            false);
    })

    .TestCase(u8"Trivial copyable types are trivially copyable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct TriviallyCopyable
        {
            TriviallyCopyable(Immutable<TriviallyCopyable>) = default;

            Mutable<TriviallyCopyable>
            operator=(Immutable<TriviallyCopyable>) = default;

            ~TriviallyCopyable() {};
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<TriviallyCopyable>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<Int, TriviallyCopyable>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<Immutable<TriviallyCopyable>>), false);
    })

    .TestCase(u8"Non-trivial copyable types are not trivially copyable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonTriviallyCopyable
        {
            NonTriviallyCopyable(Immutable<NonTriviallyCopyable>){};

            Mutable<NonTriviallyCopyable>
            operator=(Immutable<NonTriviallyCopyable>) = default;

            ~NonTriviallyCopyable() {};
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<NonTriviallyCopyable>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<Int, NonTriviallyCopyable>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<Immutable<NonTriviallyCopyable>>), false);
    })

    .TestCase(u8"Trivial movable types are trivially copyable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct TriviallyMovable
        {
            TriviallyMovable(TriviallyMovable&&) = default;

            Mutable<TriviallyMovable>
            operator=(Mutable<TriviallyMovable>) = default;

            ~TriviallyMovable() {};
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<TriviallyMovable>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<Int, TriviallyMovable>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<Immutable<TriviallyMovable>>), false);
    })

    .TestCase(u8"Non-trivial movable types are not trivially copyable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonTriviallyMovable
        {
            NonTriviallyMovable(NonTriviallyMovable&&){};

            Mutable<NonTriviallyMovable>
            operator=(Mutable<NonTriviallyMovable>) = default;

            ~NonTriviallyMovable() {};
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<NonTriviallyMovable>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<Int, NonTriviallyMovable>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTriviallyCopyable<Immutable<NonTriviallyMovable>>), false);
    })

    .TestCase(u8"Trivial types are trivial.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct Trivial
        {
            Trivial() = default;

            Trivial(Immutable<Trivial>) = default;

            Trivial(Trivial&&) = default;

            Mutable<Trivial>
            operator=(Immutable<Trivial>) = default;

            Mutable<Trivial>
            operator=(Trivial&&) = default;

            ~Trivial() = default;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsTrivial<Trivial>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsTrivial<Int, Trivial>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsTrivial<Immutable<Trivial>>), true);
    })

    .TestCase(u8"Non-trivial types are not trivial.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonTrivial
        {
            NonTrivial(){};

            NonTrivial(NonTrivial&&) = default;

            Mutable<NonTrivial>
            operator=(Immutable<NonTrivial>) = default;

            ~NonTrivial() = default;
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsTrivial<NonTrivial>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsTrivial<Int, NonTrivial>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsTrivial<Immutable<NonTrivial>>), false);
    })

    .TestCase(u8"Implicitly default constructible types are implicit default "
              u8"constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct ImplicitlyDefaultConstructible
        {
            ImplicitlyDefaultConstructible(){};
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsImplicitlyDefaultConstructible<ImplicitlyDefaultConstructible>),
            true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsImplicitlyDefaultConstructible<Int,
                                              ImplicitlyDefaultConstructible>),
            true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsImplicitlyDefaultConstructible<
                Immutable<ImplicitlyDefaultConstructible>>),
            true);
    })

    .TestCase(u8"Non-implicitly default constructible types are not implicit "
              u8"default constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonImplicitlyDefaultConstructible
        {
            explicit NonImplicitlyDefaultConstructible(){};
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsImplicitlyDefaultConstructible<
                NonImplicitlyDefaultConstructible>),
            false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsImplicitlyDefaultConstructible<
                Int, NonImplicitlyDefaultConstructible>),
            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsImplicitlyDefaultConstructible<
                Immutable<NonImplicitlyDefaultConstructible>>),
            false);
    })

    .TestCase(u8"Implicitly constructible types are implicit constructible "
              u8"from their constructor arguments.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct ImplicitlyConstructible
        {
            ImplicitlyConstructible(Int){};
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsImplicitlyConstructibleFrom<ImplicitlyConstructible, Int>),
            true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsImplicitlyConstructibleFrom<Immutable<ImplicitlyConstructible>,
                                           Int>),
            true);
    })

    .TestCase(u8"Non-implicitly default constructible types are not implicit "
              u8"default constructible.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonImplicitlyConstructible
        {
            explicit NonImplicitlyConstructible(Int){};
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsImplicitlyConstructibleFrom<NonImplicitlyConstructible, Int>),
            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsImplicitlyConstructibleFrom<
                Immutable<NonImplicitlyConstructible>, Int>),
            false);
    })

    .TestCase(u8"Types that can be compared for equality are "
              u8"equality-comparable",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct EqualityComparable
        {
            Bool operator==(Immutable<EqualityComparable>) const
            {
                return true;
            }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<EqualityComparable>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<Int, EqualityComparable>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<Immutable<EqualityComparable>>), true);
    })

    .TestCase(u8"Types that cannot be compared for equality are not "
              u8"equality-comparable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonEqualityComparable{};

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<NonEqualityComparable>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<Int, NonEqualityComparable>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparable<Immutable<NonEqualityComparable>>), false);
    })

    .TestCase(u8"Equality-comparable types are equal-comparable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct EqualityComparableBar;
        struct EqualityComparableFooBar;

        struct EqualityComparableFoo
        {
            Bool operator==(Immutable<EqualityComparableBar>) const
            {
                return true;
            }

            Bool operator==(Immutable<EqualityComparableFooBar>) const
            {
                return true;
            }
        };

        struct EqualityComparableBar
        {
            Bool operator==(Immutable<EqualityComparableFoo>) const
            {
                return true;
            }
        };

        struct EqualityComparableFooBar
        {
            Bool operator==(Immutable<EqualityComparableFoo>) const
            {
                return true;
            }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparableWith<EqualityComparableFoo,
                                      EqualityComparableBar>),
            true);

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparableWith<EqualityComparableBar,
                                      EqualityComparableFoo>),
            true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparableWith<EqualityComparableFoo,
                                      EqualityComparableBar,
                                      EqualityComparableFooBar>),
            true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparableWith<Immutable<EqualityComparableFoo>,
                                      Immutable<EqualityComparableBar>>),
            true);

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparableWith<Immutable<EqualityComparableBar>,
                                      Immutable<EqualityComparableFoo>>),
            true);
    })

    .TestCase(u8"Types that cannot be compared for equality are not "
              u8"equality-comparable.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonEqualityComparableBar;
        struct NonEqualityComparableFooBar;

        struct NonEqualityComparableFoo
        {
            Bool operator==(Immutable<NonEqualityComparableFooBar>) const
            {
                return true;
            }
        };

        struct NonEqualityComparableBar{};

        struct NonEqualityComparableFooBar
        {
            Bool operator==(Immutable<NonEqualityComparableFoo>) const
            {
                return true;
            }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparableWith<NonEqualityComparableFoo,
                                      NonEqualityComparableBar>),
            false);

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparableWith<NonEqualityComparableBar,
                                      NonEqualityComparableFoo>),
            false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparableWith<NonEqualityComparableFoo,
                                      NonEqualityComparableFooBar,
                                      NonEqualityComparableBar>),
            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparableWith<Immutable<NonEqualityComparableFoo>,
                                      Immutable<NonEqualityComparableBar>>),
            false);

        SYNTROPY_UNIT_EQUAL(
            (IsEqualityComparableWith<Immutable<NonEqualityComparableBar>,
                                      Immutable<NonEqualityComparableFoo>>),
            false);
    })

    .TestCase(u8"Types that can be compared less-than, greater-than, "
              u8"less-than-or-equal-to and greater-than-or-equal-to are "
              u8"partially-ordered.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct PartiallyOrdered
        {
            Ordering operator<=>(Immutable<PartiallyOrdered>) const
            {
                return Ordering::kEquivalent;
            }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsPartiallyOrdered<PartiallyOrdered>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsPartiallyOrdered<PartiallyOrdered, Int>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsPartiallyOrdered<Immutable<PartiallyOrdered>>), true);
    })

    .TestCase(u8"Types that cannot be compared equal, less-than, "
              u8"greater-than, less-than-or-equal-to or "
              u8"greater-than-or-equal-to are not partially-ordered.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonPartiallyOrdered {};

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsPartiallyOrdered<NonPartiallyOrdered>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsPartiallyOrdered<Int, NonPartiallyOrdered>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsPartiallyOrdered<Immutable<NonPartiallyOrdered>>), false);
    })

    .TestCase(u8"Types that can be compared less-than, greater-than, "
              u8"less-than-or-equal-to and greater-than-or-equal-to another "
              u8"type are partially-ordered.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct PartiallyOrderedBar;
        struct PartiallyOrderedFooBar;

        struct PartiallyOrderedFoo
        {
            Ordering operator<=>(Immutable<PartiallyOrderedBar>) const
            {
                return Ordering::kEquivalent;
            }

            Ordering operator<=>(Immutable<PartiallyOrderedFooBar>) const
            {
                return Ordering::kEquivalent;
            }
        };

        struct PartiallyOrderedBar
        {
            Ordering operator<=>(Immutable<PartiallyOrderedFoo>) const
            {
                return Ordering::kEquivalent;
            }
        };

        struct PartiallyOrderedFooBar
        {
            Ordering operator<=>(Immutable<PartiallyOrderedFoo>) const
            {
                return Ordering::kEquivalent;
            }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsPartiallyOrderedWith<PartiallyOrderedFoo, PartiallyOrderedBar>),
            true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsPartiallyOrderedWith<PartiallyOrderedFoo,
                                    PartiallyOrderedBar,
                                    PartiallyOrderedFooBar>),
            true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsPartiallyOrderedWith<Immutable<PartiallyOrderedFoo>,
                                    Immutable<PartiallyOrderedBar>>),
            true);
    })

    .TestCase(u8"Types that cannot be compared equal, less-than, "
              u8"greater-than, less-than-or-equal-to or "
              u8"greater-than-or-equal-to another type are not "
              u8"partially-ordered.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        struct NonPartiallyOrderedBar;
        struct NonPartiallyOrderedFooBar;

        struct NonPartiallyOrderedFoo
        {
            Ordering operator<=>(Immutable<NonPartiallyOrderedFooBar>) const
            {
                return Ordering::kEquivalent;
            }
        };

        struct NonPartiallyOrderedBar
        {


        };

        struct NonPartiallyOrderedFooBar
        {
            Ordering operator<=>(Immutable<NonPartiallyOrderedFoo>) const
            {
                return Ordering::kEquivalent;
            }
        };

        // Base case.

        SYNTROPY_UNIT_EQUAL((
            IsPartiallyOrderedWith<NonPartiallyOrderedFoo,
                                   NonPartiallyOrderedBar>),
            false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((
            IsPartiallyOrderedWith<NonPartiallyOrderedFoo,
                                   NonPartiallyOrderedFooBar,
                                   NonPartiallyOrderedBar>),
            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsPartiallyOrderedWith<Immutable<NonPartiallyOrderedFoo>,
                                    Immutable<NonPartiallyOrderedBar>>),
            false);
    })

    .TestCase(u8"Types that are template specialization of a template type "
              u8"are template-specialization-of.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateSpecializationOf<ConceptsTestFixture::Template<Int>,
                                        ConceptsTestFixture::Template>),
            true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateSpecializationOf<
                Immutable<ConceptsTestFixture::Template<Int>>,
                ConceptsTestFixture::Template>),
            true);
    })

    .TestCase(u8"Types that are not template specialization of a template "
              u8"type are not template-specialization-of.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateSpecializationOf<ConceptsTestFixture::Template2<Int>,
                                        ConceptsTestFixture::Template>),
            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateSpecializationOf<
                Immutable<ConceptsTestFixture::Template2<Int>>,
                ConceptsTestFixture::Template>),
            false);
    })

    .TestCase(u8"Types that are template of a specialized template type "
              u8"are template-of those classes.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateOf<ConceptsTestFixture::Template,
                          ConceptsTestFixture::Template<Int>>),
            true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateOf<ConceptsTestFixture::Template,
                          ConceptsTestFixture::Template<Int>,
                          ConceptsTestFixture::Template<Float>>),
            true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateOf<ConceptsTestFixture::Template,
                          Immutable<ConceptsTestFixture::Template<Int>>>),
            true);
    })

    .TestCase(u8"Types that are not template of a template specialization "
              u8"type are not template-of.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateOf<ConceptsTestFixture::Template,
                          ConceptsTestFixture::Template2<Int>>),
            false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateOf<ConceptsTestFixture::Template,
                          ConceptsTestFixture::Template<Int>,
                          ConceptsTestFixture::Template2<Int>>),
            false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsTemplateOf<ConceptsTestFixture::Template,
                          Immutable<ConceptsTestFixture::Template2<Int>>>),
            false);
    })

    .TestCase(u8"Empty TypeList are typelists.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsTypeList<TypeList<>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsTypeList<TypeList<>, TypeList<>>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsTypeList<Immutable<TypeList<>>>), true);
    })

    .TestCase(u8"Non-empty TypeList are typelists",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsTypeList<TypeList<Int, Float>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsTypeList<TypeList<Int>, TypeList<Float>>), true);
    })

    .TestCase(u8"Types other than TypeList are not typelists.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsTypeList<Int>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsTypeList<TypeList<>, Float>), false);
    })

    .TestCase(u8"Sequence types are sequence.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsSequence<Sequence<1, 3, 5>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsSequence<Sequence<1, 3, 5>, Sequence<1>>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsSequence<Immutable<Sequence<1, 3, 5>>>), true);
    })

    .TestCase(u8"Non sequence types are not sequence.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL((IsSequence<Int>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL((IsSequence<Sequence<1, 3, 5>, Int>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL((IsSequence<Immutable<Int>>), false);
    })

    .TestCase(u8"Contiguous sequence types are contiguous sequence.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Sequence<1, 2, 3>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Sequence<1, 2, 3>, Sequence<4, 5>>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Immutable<Sequence<1, 2, 3>>>), true);
    })

    .TestCase(u8"Zero-element sequences are not contiguous sequences",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Sequence<>>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Sequence<1, 2>, Sequence<>>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Immutable<Sequence<>>>), false);
    })

    .TestCase(u8"One-element sequences are contiguous sequences",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Sequence<1>>), true);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Sequence<1>, Sequence<5>>), true);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Immutable<Sequence<1>>>), true);
    })

    .TestCase(u8"Non-contiguous sequence types are not contiguous sequence.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        // Base case.

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Sequence<1, 3, 2>>), false);

        // Variadic case.

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Sequence<1, 2, 3>, Sequence<3, 2>>), false);

        // Reference types should not affect result.

        SYNTROPY_UNIT_EQUAL(
            (IsContiguousSequence<Immutable<Sequence<1, 3, 2>>>), false);
    });

}

// ===========================================================================
