
/// \file compare_unit_test.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/diagnostics/unit_test/unit_test.h"

#include "syntropy/core/comparisons/compare.h"
#include "syntropy/core/comparisons/compare_extensions.h"
#include "syntropy/core/comparisons/ordering.h"

#include "syntropy/math/numbers.h"

// ===========================================================================

namespace CompareUnitTest
{
    namespace Sy = Syntropy;

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    /// \brief Generic comparable type via member-functions.
    struct MemberComparable
    {
        Sy::Bool
        IsEqual(Sy::Immutable<MemberComparable>) const;

        Sy::Bool
        IsEquivalent(Sy::Immutable<MemberComparable>) const;

        Sy::Ordering
        Compare(Sy::Immutable<MemberComparable>) const;
    };

    /// \brief Generic comparable type via non-member functions.
    struct NonMemberComparable {};

    /// \brief Generic comparable type via operators.
    struct OperatorComparable {};

    // Non-mmember functions.
    // ======================

    Sy::Bool
    AreEqual(Sy::Immutable<NonMemberComparable>,
             Sy::Immutable<NonMemberComparable>);

    Sy::Bool
    AreEquivalent(Sy::Immutable<NonMemberComparable>,
                  Sy::Immutable<NonMemberComparable>);

    Sy::Ordering
    Compare(Sy::Immutable<NonMemberComparable>,
            Sy::Immutable<NonMemberComparable>);

    // Non-member operators for OperatorComparable.

    Sy::Bool
    operator==(Sy::Immutable<OperatorComparable>,
               Sy::Immutable<OperatorComparable>);

    Sy::Ordering
    operator<=>(Sy::Immutable<OperatorComparable>,
                Sy::Immutable<OperatorComparable>);

    /************************************************************************/
    /* FIXTURE                                                              */
    /************************************************************************/

    /// \brief Fixture.
    struct Fixture {};

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& compare_unit_test
        = Sy::UnitTest::MakeAutoUnitTest<Fixture>(
            u8"compare.algorithms.core.syntropy")

    .TestCase(u8"Integral types support comparison.",
    [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEqual(1, 1), true);

        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEqual(1, 2), false);
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEqual(2, 1), false);

        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEquivalent(1, 1), true);

        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEquivalent(1, 2), false);
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEquivalent(2, 1), false);

        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::Compare(1, 1), Sy::Ordering::kEquivalent);
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::Compare(1, 2), Sy::Ordering::kLess);
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::Compare(2, 1), Sy::Ordering::kGreater);
    })

    .TestCase(u8"Floating-point types support comparison.",
    [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEqual(1.0f, 1.0f), true);

        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEqual(1.0f, 2.0f), false);
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEqual(2.0f, 1.0f), false);

        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEquivalent(1.0f, 1.0f), true);

        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEquivalent(1.0f, 2.0f), false);
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEquivalent(2.0f, 1.0f), false);

        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::Compare(1.0f, 1.0f), Sy::Ordering::kEquivalent);

        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::Compare(1.0f, 1.0f), Sy::Ordering::kEquivalent);
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::Compare(1.0f, 2.0f), Sy::Ordering::kLess);
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::Compare(2.0f, 1.0f), Sy::Ordering::kGreater);
    })

    .TestCase(u8"Positive and negative floating-point zero compare "
              u8"equivalent.",
    [](auto& fixture)
    {
        // Positive and negative zeroes are distinguishable but partial
        // ordering doesn't provide a way to distinguish the two.

        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEquivalent(+0.0f, -0.0f), true);
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEqual(+0.0f, -0.0f), true);
    })

    .TestCase(u8"Any number is smaller than the positive infinity.",
    [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::Compare(0.0f, Sy::Math::Infinity()),
                            Sy::Ordering::kLess);
    })

    .TestCase(u8"Any number is greater than the negative infinity.",
    [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::Compare(0.0f, -Sy::Math::Infinity()),
                            Sy::Ordering::kGreater);
    })

    .TestCase(u8"Infinity is equal to itself.",
    [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::Compare(Sy::Math::Infinity(), Sy::Math::Infinity()),
                            Sy::Ordering::kEquivalent);
    })

    .TestCase(u8"Not-a-number is not equal or equivalent to itself.",
    [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEqual(Sy::Math::NotANumber(), Sy::Math::NotANumber()),
                            false);

        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::AreEqual(Sy::Math::NotANumber(), Sy::Math::NotANumber()),
                            false);
    })

    .TestCase(u8"Not-a-number is incomparable with anything else.",
    [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::Compare(0.0f, Sy::Math::NotANumber()),
                            Sy::Ordering::kIncomparable);

        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::Compare(Sy::Math::NotANumber(), 0.0f),
                            Sy::Ordering::kIncomparable);

        SYNTROPY_UNIT_EQUAL(Sy::Comparisons::Compare(Sy::Math::NotANumber(), Sy::Math::NotANumber()),
                            Sy::Ordering::kIncomparable);
    });

}

// ===========================================================================

#include "details/compare_unit_test.inl"

// ===========================================================================
