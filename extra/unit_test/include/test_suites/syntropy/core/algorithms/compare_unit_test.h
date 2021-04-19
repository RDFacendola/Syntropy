
/// \file compare_unit_test.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/diagnostics/unit_test/unit_test.h"

#include "syntropy/core/algorithms/compare.h"
#include "syntropy/core/algorithms/compare_extensions.h"
#include "syntropy/core/foundation/ordering.h"

#include "syntropy/math/numbers.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* COMPARE TEST FIXTURE                                                 */
    /************************************************************************/

    /// \brief Compare test fixture.
    struct CompareTestFixture
    {

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& compare_unit_test
        = Syntropy::UnitTest::MakeAutoUnitTest<CompareTestFixture>(
            u8"compare.algorithms.core.syntropy")

    .TestCase(u8"Integral types support comparison.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Algorithms;

        SYNTROPY_UNIT_EQUAL(AreEqual(1, 1), true);

        SYNTROPY_UNIT_EQUAL(AreEqual(1, 2), false);
        SYNTROPY_UNIT_EQUAL(AreEqual(2, 1), false);

        SYNTROPY_UNIT_EQUAL(AreEquivalent(1, 1), true);

        SYNTROPY_UNIT_EQUAL(AreEquivalent(1, 2), false);
        SYNTROPY_UNIT_EQUAL(AreEquivalent(2, 1), false);

        SYNTROPY_UNIT_EQUAL(Compare(1, 1), Ordering::kEquivalent);
        SYNTROPY_UNIT_EQUAL(Compare(1, 2), Ordering::kLess);
        SYNTROPY_UNIT_EQUAL(Compare(2, 1), Ordering::kGreater);
    })

    .TestCase(u8"Floating-point types support comparison.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Algorithms;

        SYNTROPY_UNIT_EQUAL(AreEqual(1.0f, 1.0f), true);

        SYNTROPY_UNIT_EQUAL(AreEqual(1.0f, 2.0f), false);
        SYNTROPY_UNIT_EQUAL(AreEqual(2.0f, 1.0f), false);

        SYNTROPY_UNIT_EQUAL(AreEquivalent(1.0f, 1.0f), true);

        SYNTROPY_UNIT_EQUAL(AreEquivalent(1.0f, 2.0f), false);
        SYNTROPY_UNIT_EQUAL(AreEquivalent(2.0f, 1.0f), false);

        SYNTROPY_UNIT_EQUAL(Compare(1.0f, 1.0f), Ordering::kEquivalent);

        SYNTROPY_UNIT_EQUAL(Compare(1.0f, 1.0f), Ordering::kEquivalent);
        SYNTROPY_UNIT_EQUAL(Compare(1.0f, 2.0f), Ordering::kLess);
        SYNTROPY_UNIT_EQUAL(Compare(2.0f, 1.0f), Ordering::kGreater);
    })

    .TestCase(u8"Positive and negative floating-point zero compare "
              u8"equivalent.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Algorithms;

        // Positive and negative zeroes are distinguishable but partial
        // ordering doesn't provide a way to distinguish the two.

        SYNTROPY_UNIT_EQUAL(AreEquivalent(+0.0f, -0.0f), true);
        SYNTROPY_UNIT_EQUAL(AreEqual(+0.0f, -0.0f), true);
    })

    .TestCase(u8"Any number is smaller than the positive infinity.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Algorithms;

        SYNTROPY_UNIT_EQUAL(Compare(0.0f, Math::Infinity()),
                            Ordering::kLess);
    })

    .TestCase(u8"Any number is greater than the negative infinity.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Algorithms;

        SYNTROPY_UNIT_EQUAL(Compare(0.0f, -Math::Infinity()),
                            Ordering::kGreater);
    })

    .TestCase(u8"Infinity is equal to itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Algorithms;

        SYNTROPY_UNIT_EQUAL(Compare(Math::Infinity(), Math::Infinity()),
                            Ordering::kEquivalent);
    })

    .TestCase(u8"Not-a-number is not equal or equivalent to itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Algorithms;

        SYNTROPY_UNIT_EQUAL(AreEqual(Math::NotANumber(), Math::NotANumber()),
                            false);

        SYNTROPY_UNIT_EQUAL(AreEqual(Math::NotANumber(), Math::NotANumber()),
                            false);
    })

    .TestCase(u8"Not-a-number is incomparable with anything else.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Algorithms;

        SYNTROPY_UNIT_EQUAL(Compare(0.0f, Math::NotANumber()),
                            Ordering::kIncomparable);

        SYNTROPY_UNIT_EQUAL(Compare(Math::NotANumber(), 0.0f),
                            Ordering::kIncomparable);

        SYNTROPY_UNIT_EQUAL(Compare(Math::NotANumber(), Math::NotANumber()),
                            Ordering::kIncomparable);
    });

}

// ===========================================================================
