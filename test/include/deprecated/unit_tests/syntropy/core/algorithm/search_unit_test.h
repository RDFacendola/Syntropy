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
        Span<Int> ints_span_;

        /// \brief Empty integer sequence span.
        Span<Int> empty_span_;

        /// \brief Executed before each test case.
        void Before();

        /// \brief Predicates which is defined true if the provided argument is odd, false otherwise.
        static Bool IsOdd(Int x) noexcept;

        /// \brief Predicates which is defined true if the provided argument is even, false otherwise.
        static Bool IsEven(Int x) noexcept;

        /// \brief Predicates which is defined false for every provided argument.
        static Bool Never(Int x) noexcept;
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& search_unit_test = MakeAutoUnitTest<SearchTestFixture>("search.algorithm.core.syntropy")

    .TestCase("Searching a range for a value returns a range starting with that value and the remaining ones.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Algorithm::Find(fixture.ints_span_, 4), (Span<Int>{ &fixture.ints_[4], 6}));
    })

    .TestCase("Searching a range for a non-existent value returns an empty range.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(IsEmpty(Algorithm::Find(fixture.ints_span_, 11)), true);
    })

    .TestCase("Reverse-searching a range for a value returns a range ending with that value and all elements prior to that.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Algorithm::FindReverse(fixture.ints_span_, 4), (Span<Int>{ &fixture.ints_[0], 5}));
    })

    .TestCase("Searching a reverse range for a non-existent value returns an empty range.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(IsEmpty(Algorithm::FindReverse(fixture.ints_span_, 11)), true);
    })

     .TestCase("Searching a range by predicate returns a range starting with the first element for which the predicate holds true and all values after that.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Algorithm::FindIf(fixture.ints_span_, &SearchTestFixture::IsOdd), (Span<Int>{ &fixture.ints_[1], 9}));
    })

    .TestCase("Searching a range by a predicate which is false for all the range elements returns an empty range.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(IsEmpty(Algorithm::FindIf(fixture.ints_span_, &SearchTestFixture::Never)), true);
    })

    .TestCase("Reverse-searching a range by predicate returns a range ending with the last element for which the predicate holds true and all values before that.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Algorithm::FindIfReverse(fixture.ints_span_, &SearchTestFixture::IsEven), (Span<Int>{ &fixture.ints_[0], 9}));
    })

    .TestCase("Reverse-searching a range by a predicate which is false for all the range elements returns an empty range.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(IsEmpty(Algorithm::FindIfReverse(fixture.ints_span_, &SearchTestFixture::Never)), true);
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

    inline Bool SearchTestFixture::IsOdd(Int x) noexcept
    {
        return (x % 2) == 1;
    }

    inline Bool SearchTestFixture::IsEven(Int x) noexcept
    {
        return (x % 2) == 0;
    }

    inline Bool SearchTestFixture::Never(Int x) noexcept
    {
        return false;
    }
}

// ===========================================================================