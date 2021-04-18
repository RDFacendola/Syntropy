
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

    .TestCase(u8"Real types support comparison.",
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
        SYNTROPY_UNIT_EQUAL(Compare(1.0f, 2.0f), Ordering::kLess);
        SYNTROPY_UNIT_EQUAL(Compare(2.0f, 1.0f), Ordering::kGreater);
    })

}

// ===========================================================================
