
#/// \file compare_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/experimental/language/support/compare.h"

#include "syntropy/unit_test/unit_test.h"

#include <compare>

namespace Syntropy::UnitTest
{
    /************************************************************************/
    /* COMPARE TEST FIXTURE                                                 */
    /************************************************************************/

    /// \brief Compare test fixture.
    struct CompareTestFixture
    {
        /// \brief Definition for a strongly-comparable class.
        struct StrongComparable
        {
            Int value_;

            Bool operator==(Reference<StrongComparable> rhs)
            {
                return true;
            }

            StrongOrdering operator<=>(Reference<StrongComparable> rhs) const noexcept
            {
                return StrongOrdering::Equal;
            }
        };
    };

    /************************************************************************/
    /* UNIT TESTS                                                           */
    /************************************************************************/

    inline const auto& kCompareUnitTest = MakeAutoUnitTest<CompareTestFixture>("compare.support.language.syntropy")

    .TestCase("Equal instances of a strongly-comparable class compare equal/equivalent.", [](auto& fixture)
    {
        auto foo = CompareTestFixture::StrongComparable{ 0 };
        auto bar = CompareTestFixture::StrongComparable{ 0 };

        SYNTROPY_UNIT_EQUAL(foo <=> bar, StrongOrdering::Equal);
        SYNTROPY_UNIT_EQUAL(foo <=> bar, StrongOrdering::Equivalent);

        //

        SYNTROPY_UNIT_EQUAL(foo == bar, true);
        SYNTROPY_UNIT_EQUAL(foo != bar, false);
        SYNTROPY_UNIT_EQUAL(foo < bar, false);
        SYNTROPY_UNIT_EQUAL(foo > bar, false);
        SYNTROPY_UNIT_EQUAL(foo <= bar, true);
        SYNTROPY_UNIT_EQUAL(foo >= bar, true);
    })

    .TestCase("Instances of a strongly-comparable in which the left-hand-side is less than the right-hand- side class compare less-than. ", [](auto& fixture)
    {
        auto foo = CompareTestFixture::StrongComparable{ 0 };
        auto bar = CompareTestFixture::StrongComparable{ 1 };

        SYNTROPY_UNIT_EQUAL(foo <=> bar, StrongOrdering::Less);

        //

        SYNTROPY_UNIT_EQUAL(foo == bar, false);
        SYNTROPY_UNIT_EQUAL(foo != bar, true);
        SYNTROPY_UNIT_EQUAL(foo < bar, true);
        SYNTROPY_UNIT_EQUAL(foo > bar, false);
        SYNTROPY_UNIT_EQUAL(foo <= bar, true);
        SYNTROPY_UNIT_EQUAL(foo >= bar, false);
    })

    .TestCase("Instances of a strongly-comparable in which the left-hand-side is greater than the right-hand- side class compare greater-than. ", [](auto& fixture)
    {
        auto foo = CompareTestFixture::StrongComparable{ 1 };
        auto bar = CompareTestFixture::StrongComparable{ 0 };

        SYNTROPY_UNIT_EQUAL(foo <=> bar, StrongOrdering::Greater);

        //

        SYNTROPY_UNIT_EQUAL(foo == bar, false);
        SYNTROPY_UNIT_EQUAL(foo != bar, true);
        SYNTROPY_UNIT_EQUAL(foo < bar, false);
        SYNTROPY_UNIT_EQUAL(foo > bar, true);
        SYNTROPY_UNIT_EQUAL(foo <= bar, false);
        SYNTROPY_UNIT_EQUAL(foo >= bar, true);
    });
}