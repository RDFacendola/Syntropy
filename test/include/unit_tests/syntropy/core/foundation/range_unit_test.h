#/// \file range_unit_test.h
///
/// \author Raffaele D. Facendola - November 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/containers/fix_array.h"
#include "syntropy/core/foundation/span.h"
#include "syntropy/experimental/core/foundation/range.h"

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

        /// \brief Empty integer sequence span.
        SpanT<Int> empty_span_;

        /// \brief Executed before each test case.
        void Before();
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& range_unit_test = MakeAutoUnitTest<RangeTestFixture>("range.foundation.core.syntropy")

    .TestCase("Iterating an empty range calls the provided function exactly 0 times.", [](auto& fixture)
    {
        ForEach(fixture.empty_span_, [](Int)
        {
            SYNTROPY_UNIT_EQUAL(true, false);
        });
    })

    .TestCase("Iterating a range visits each element in the same order specified by the range.", [](auto& fixture)
    {
        auto index = ToInt(0);

        ForEach(fixture.ints_span_, [&index](Int element)
        {
            SYNTROPY_UNIT_EQUAL(element, index);

            ++index;
        });

        SYNTROPY_UNIT_EQUAL(index, 10);
    })

    .TestCase("Iterating a reverse empty range calls the provided function exactly 0 times.", [](auto& fixture)
    {
        ForEach(Reverse(fixture.empty_span_), [](Int)
        {
            SYNTROPY_UNIT_EQUAL(true, false);
        });
    })

    .TestCase("Iterating a reversed range visits each element in the reverse order specified by the original range.", [](auto& fixture)
    {
        auto index = ToInt(9);

        ForEach(Reverse(fixture.ints_span_), [&index](Int element)
        {
            SYNTROPY_UNIT_EQUAL(element, index);

            --index;
        });

        SYNTROPY_UNIT_EQUAL(index, -1);
    })

    .TestCase("Reversing a range twice returns the original range.", [](auto& fixture)
    {
        auto index = ToInt(0);

        ForEach(Reverse(Reverse(fixture.ints_span_)), [&index](Int element)
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
        ints_span_ = { &ints_[0], 10 };
        empty_span_ = {};
    }

}

// ===========================================================================