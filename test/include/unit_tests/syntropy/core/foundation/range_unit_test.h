#/// \file range_unit_test.h
///
/// \author Raffaele D. Facendola - November 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/containers/fix_array.h"
#include "syntropy/core/foundation/span.h"
#include "syntropy/core/foundation/range.h"

#include "syntropy/unit_test/unit_test.h"

// ===========================================================================

namespace Syntropy::UnitTest
{
    /************************************************************************/
    /* RANGE TEST FIXTURE                                                   */
    /************************************************************************/

    /// \brief Span test fixture.
    struct RangeTestFixture
    {
        /// \brief Integer sequence.
        FixArray<Int, 10> ints_ = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        /// \brief Integer sequence span.
        SpanT<Int> ints_span_;

        /// \brief Executed before each test case.
        void Before();
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& range_unit_test = MakeAutoUnitTest<RangeTestFixture>("range.foundation.core.syntropy")

    .TestCase("Iterating a range visits each element in the same order specified by the range.", [](auto& fixture)
    {
        auto index = ToInt(0);

        Algorithm::ForEach(fixture.ints_span_, [&index](Int element)
        {
            SYNTROPY_UNIT_EQUAL(element, index);

            ++index;
        });

        SYNTROPY_UNIT_EQUAL(index, 10);
    });

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // SpanTestFixture.

    inline void RangeTestFixture::Before()
    {
        ints_span_ = SpanT<Int>{ &ints_[0], 10 };
    }

}

// ===========================================================================