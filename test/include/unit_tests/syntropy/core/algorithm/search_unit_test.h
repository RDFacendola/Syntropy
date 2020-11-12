#/// \file range_unit_test.h
///
/// \author Raffaele D. Facendola - November 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/containers/fix_array.h"
#include "syntropy/core/foundation/span.h"

#include "syntropy/experimental/core/foundation/range.h"
#include "syntropy/experimental/core/algorithm/search.h"

#include "syntropy/unit_test/unit_test.h"

// ===========================================================================

namespace Syntropy::Algorithm::UnitTest
{
    /************************************************************************/
    /* SEARCH TEST FIXTURE                                                  */
    /************************************************************************/

    /// \brief Search test fixture.
    struct SearchTestFixture
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

    inline const auto& search_unit_test = MakeAutoUnitTest<SearchTestFixture>("search.algorithm.core.syntropy")

    .TestCase("Searching a range for a value returns a range starting with that value and the remaining ones.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Algorithm::Find(fixture.ints_span_, 4), (SpanT<Int>{ &fixture.ints_[4], 6}));
    })

    .TestCase("Searching a range for a non-existent value returns an empty range.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(IsEmpty(Algorithm::Find(fixture.ints_span_, 11)), true);
    })

    .TestCase("Reverse-searching a range for a value returns a range ending with that value and all elements prior to that.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Algorithm::FindReverse(fixture.ints_span_, 4), (SpanT<Int>{ &fixture.ints_[0], 5}));
    })

    .TestCase("Searching a reverse range for a non-existent value returns an empty range.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(IsEmpty(Algorithm::FindReverse(fixture.ints_span_, 11)), true);
    });

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // SearchTestFixture.

    inline void SearchTestFixture::Before()
    {
        ints_span_ = { &ints_[0], 10 };
        empty_span_ = {};
    }

}

// ===========================================================================