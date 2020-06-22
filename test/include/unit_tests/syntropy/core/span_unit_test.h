/// \file span_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/core/types.h"

#include "syntropy/experimental/core/span.h"

#include "syntropy/unit_test/test_macros.h"
#include "syntropy/unit_test/auto_unit_test.h"

namespace syntropy::unit_test
{
    /************************************************************************/
    /* SPAN TEST FIXTURE                                                    */
    /************************************************************************/

    /// \brief Span test fixture.
    struct SpanTestFixture
    {
        /// \brief Integer sequence.
        Int int_sequence_[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        /// \brief Float sequence.
        Float float_sequence_[10] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };

        /// \brief Integer constant sequence.
        Int const_sequence_[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

        /// \brief Executed before each test case.
        void Before();

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& span_unit_test = MakeAutoUnitTest<SpanTestFixture>("Span")

    .TestCase("Default constructed span is empty", [](auto& fixture)
    {
        auto span = Span<Int>{};

        SYNTROPY_UNIT_EQUAL(span.GetCount(), 0);
        SYNTROPY_UNIT_EQUAL(Count(span), 0);
        SYNTROPY_UNIT_EQUAL(!span, true);
        SYNTROPY_UNIT_EQUAL(IsEmpty(span), true);
    })

    .TestCase("A span constructed from a pair of non-equal iterator is non-empty ", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], &fixture.int_sequence_[9] + 1 };

        SYNTROPY_UNIT_EQUAL(span.GetCount(), 10);
        SYNTROPY_UNIT_EQUAL(Count(span), 10);
        SYNTROPY_UNIT_EQUAL(!!span, true);
        SYNTROPY_UNIT_EQUAL(IsEmpty(span), false);
    })

    .TestCase("A span constructed from a pointer and a non-zero number of elements is non-empty", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(span.GetCount(), 10);
        SYNTROPY_UNIT_EQUAL(!!span, true);
        SYNTROPY_UNIT_EQUAL(IsEmpty(span), false);
    })

    .TestCase("Each element in a span provides read-only random access", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(span[5], 5);
    })

    .TestCase("Each non-const element in a span provides read-write random access", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        span[5] = 42;

        SYNTROPY_UNIT_EQUAL(span[5], 42);
    })

    .TestCase("Spans provide both equality and inequality comparison.", [](auto& fixture)
    {
        auto span1 = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span2 = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span3 = Span<Int>{ &fixture.int_sequence_[1], 8 };

        SYNTROPY_UNIT_EQUAL(span1 == span1, true);
        SYNTROPY_UNIT_EQUAL(span1 == span2, true);
        SYNTROPY_UNIT_EQUAL(span1 != span3, true);
    })

    .TestCase("Front element is both readable and writable.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Front(span), 0);

        Front(span) = 42;

        SYNTROPY_UNIT_EQUAL(Front(span), 42);
    })

    .TestCase("Back element is both readable and writable.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Back(span), 9);

        Back(span) = 42;

        SYNTROPY_UNIT_EQUAL(Back(span), 42);
    })

    .TestCase("Subspan returns the elements inside the span that was used to generate it.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto subspan = Span<Int>{ &fixture.int_sequence_[3], 5 };

        SYNTROPY_UNIT_EQUAL(Subspan(span, 3, 5), subspan);
    })

    .TestCase("Full sub-spans are equal to the span that was used to generate them.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Subspan(span, 0, 10), span);
    })

    .TestCase("Empty sub-spans are equal to any empty span.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto empty_span = Span<Int>{};

        SYNTROPY_UNIT_EQUAL(Subspan(span, 0, 0), empty_span);
    })

    .TestCase("Removing elements from the span front yields a sub-span which is equal to the remaining elements.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto subspan1 = Span<Int>{ &fixture.int_sequence_[1], 9 };
        auto subspan3 = Span<Int>{ &fixture.int_sequence_[3], 7 };

        SYNTROPY_UNIT_EQUAL(PopFront(span), subspan1);
        SYNTROPY_UNIT_EQUAL(PopFront(span, 3), subspan3);
    })

    .TestCase("Removing elements from the span back yields a sub-span which is equal to the remaining elements.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto subspan1 = Span<Int>{ &fixture.int_sequence_[0], 9 };
        auto subspan3 = Span<Int>{ &fixture.int_sequence_[0], 7 };

        SYNTROPY_UNIT_EQUAL(PopBack(span), subspan1);
        SYNTROPY_UNIT_EQUAL(PopBack(span, 3), subspan3);
    })

    .TestCase("Selecting the first elements of a span yields a sub-span which has the selected elements only.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto subspan = Span<Int>{ &fixture.int_sequence_[0], 4 };

        SYNTROPY_UNIT_EQUAL(First(span, 4), subspan);
    })

    .TestCase("Selecting the last elements of a span yields a sub-span which has the selected elements only.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto subspan = Span<Int>{ &fixture.int_sequence_[6], 4 };

        SYNTROPY_UNIT_EQUAL(Last(span, 4), subspan);
    })

    .TestCase("Spans are strongly equivalent when they refer to the same memory region, and weakly equivalent when their elements have the same value representation.", [](auto& fixture)
    {
        auto span1 = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span2 = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span3 = Span<Int>{ &fixture.int_sequence_[2], 8 };
        auto spanf = Span<Float>{ &fixture.float_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(EqualsStrong(span1, span2), true);
        SYNTROPY_UNIT_EQUAL(EqualsWeak(span1, span2), true);

        SYNTROPY_UNIT_EQUAL(EqualsStrong(span1, span3), false);
        SYNTROPY_UNIT_EQUAL(EqualsWeak(span1, span3), false);

        SYNTROPY_UNIT_EQUAL(EqualsWeak(span1, spanf), true);
    })

    .TestCase("Spans have strong prefixes when they refer to the same memory region, and weak prefixes when their elements have the same value representation.", [](auto& fixture)
    {
        auto span1 = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span2 = Span<Int>{ &fixture.int_sequence_[0], 3 };
        auto span3 = Span<Int>{ &fixture.int_sequence_[2], 8 };
        auto spanf = Span<Float>{ &fixture.float_sequence_[0], 3 };

        SYNTROPY_UNIT_EQUAL(HasPrefixStrong(span1, span2), true);
        SYNTROPY_UNIT_EQUAL(HasPrefixWeak(span1, span2), true);

        SYNTROPY_UNIT_EQUAL(HasPrefixStrong(span1, span3), false);
        SYNTROPY_UNIT_EQUAL(HasPrefixWeak(span1, span3), false);

        SYNTROPY_UNIT_EQUAL(HasPrefixWeak(span1, spanf), true);
    })

    .TestCase("Spans have strong suffixes when they refer to the same memory region, and weak suffixes when their elements have the same value representation.", [](auto& fixture)
    {
        auto span1 = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span2 = Span<Int>{ &fixture.int_sequence_[7], 3 };
        auto span3 = Span<Int>{ &fixture.int_sequence_[2], 5 };
        auto spanf = Span<Float>{ &fixture.float_sequence_[7], 3 };

        SYNTROPY_UNIT_EQUAL(HasSuffixStrong(span1, span2), true);
        SYNTROPY_UNIT_EQUAL(HasSuffixWeak(span1, span2), true);

        SYNTROPY_UNIT_EQUAL(HasSuffixStrong(span1, span3), false);
        SYNTROPY_UNIT_EQUAL(HasSuffixWeak(span1, span3), false);

        SYNTROPY_UNIT_EQUAL(HasSuffixWeak(span1, spanf), true);
    })

    .TestCase("Spans have strong sub-spans when they refer to the same memory region, and weak sub-spans when their elements have the same value representation.", [](auto& fixture)
    {
        auto span1 = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span2 = Span<Int>{ &fixture.int_sequence_[2], 4 };
        auto span3 = Span<Int>{ &fixture.const_sequence_[2], 3 };
        auto spanf = Span<Float>{ &fixture.float_sequence_[4], 2 };

        SYNTROPY_UNIT_EQUAL(ContainsStrong(span1, span2), true);
        SYNTROPY_UNIT_EQUAL(ContainsWeak(span1, span2), true);

        SYNTROPY_UNIT_EQUAL(ContainsStrong(span1, span3), false);
        SYNTROPY_UNIT_EQUAL(ContainsWeak(span1, span3), false);

        SYNTROPY_UNIT_EQUAL(ContainsWeak(span1, spanf), true);
    })

    .TestCase("Spans have strong sub-spans when they refer to the same memory region, and weak sub-spans when their elements have the same value representation.", [](auto& fixture)
    {
        auto span1 = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span2 = Span<Int>{ &fixture.int_sequence_[2], 4 };
        auto spanf = Span<Float>{ &fixture.float_sequence_[2], 4 };

        auto span12 = Span<Int>{ &fixture.int_sequence_[2], 8 };
        auto span12f = Span<Float>{ &fixture.float_sequence_[2], 8 };

        auto span3 = Span<Int>{ &fixture.const_sequence_[2], 3 };

        SYNTROPY_UNIT_EQUAL(EqualsStrong(SearchStrong(span1, span2), span12), true);
        SYNTROPY_UNIT_EQUAL(EqualsWeak(SearchWeak(span1, span12), span12), true);

        SYNTROPY_UNIT_EQUAL(IsEmpty(SearchStrong(span1, span3)), true);
        SYNTROPY_UNIT_EQUAL(IsEmpty(SearchWeak(span1, span3)), true);

        SYNTROPY_UNIT_EQUAL(EqualsWeak(SearchWeak(span1, spanf), span12f), true);
    })

    .TestCase("Searching for an empty span (needle) in another span (haystack) returns the haystack.", [](auto& fixture)
    {
        auto haystack = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto needle = Span<Int>{};

        SYNTROPY_UNIT_EQUAL(EqualsStrong(SearchStrong(haystack, needle), haystack), true);
        SYNTROPY_UNIT_EQUAL(EqualsStrong(SearchWeak(haystack, needle), haystack), true);
    })

        ;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // SpanTestFixture.

    inline void SpanTestFixture::Before()
    {
        for (auto index = 0; index < 10; ++index)
        {
            int_sequence_[index] = index;
            float_sequence_[index] = ToFloat(index);
            const_sequence_[index] = 1;
        }
    }

}