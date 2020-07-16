/// \file span_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/span.h"

#include "syntropy/unit_test/unit_test.h"

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

        /// \brief Integer sequence.
        Int int_sequence_alt_[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        /// \brief Executed before each test case.
        void Before();
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& span_unit_test = MakeAutoUnitTest<SpanTestFixture>("span.core.syntropy")

    .TestCase("Default constructed spans are empty.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{};
        auto cspan = Span<Int>{};

        SYNTROPY_UNIT_EQUAL(!span, true);
        SYNTROPY_UNIT_EQUAL(span.GetCount(), 0);
        SYNTROPY_UNIT_EQUAL(span.GetData(), nullptr);

        SYNTROPY_UNIT_EQUAL(!cspan, true);
        SYNTROPY_UNIT_EQUAL(cspan.GetCount(), 0);
        SYNTROPY_UNIT_EQUAL(cspan.GetData(), nullptr);
    })

    .TestCase("Spans constructed from an iterator and a non-zero number of elements are non-empty.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(!!span, true);
        SYNTROPY_UNIT_EQUAL(span.GetCount(), 10);
        SYNTROPY_UNIT_EQUAL(span.GetData(), &fixture.int_sequence_[0]);

        SYNTROPY_UNIT_EQUAL(!!cspan, true);
        SYNTROPY_UNIT_EQUAL(cspan.GetCount(), 10);
        SYNTROPY_UNIT_EQUAL(cspan.GetData(), &fixture.int_sequence_[0]);
    })

    .TestCase("Spans constructed from a pair of non-equal iterator are non-empty.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], &fixture.int_sequence_[0] + 10 };
        auto cspan = Span<Int>{ &fixture.int_sequence_[0], &fixture.int_sequence_[0] + 10 };

        SYNTROPY_UNIT_EQUAL(!!span, true);
        SYNTROPY_UNIT_EQUAL(span.GetCount(), 10);
        SYNTROPY_UNIT_EQUAL(span.GetData(), &fixture.int_sequence_[0]);

        SYNTROPY_UNIT_EQUAL(!!cspan, true);
        SYNTROPY_UNIT_EQUAL(cspan.GetCount(), 10);
        SYNTROPY_UNIT_EQUAL(cspan.GetData(), &fixture.int_sequence_[0]);
    })

    .TestCase("Spans provide read-only access to elements.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(span[5], 5);
        SYNTROPY_UNIT_EQUAL(cspan[5], 5);
    })

    .TestCase("Non-constant spans provide read-write access to elements.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };

        span[5] = 42;

        SYNTROPY_UNIT_EQUAL(span[5], 42);
    })

    .TestCase("Spans are always equivalent to themselves.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(span == span, true);
        SYNTROPY_UNIT_EQUAL(span != span, false);

        SYNTROPY_UNIT_EQUAL(Equals(span, span), true);

        SYNTROPY_UNIT_EQUAL(cspan == cspan, true);
        SYNTROPY_UNIT_EQUAL(cspan != cspan, false);

        SYNTROPY_UNIT_EQUAL(Equals(cspan, cspan), true);

        SYNTROPY_UNIT_EQUAL(cspan == span, true);
        SYNTROPY_UNIT_EQUAL(cspan != span, false);

        SYNTROPY_UNIT_EQUAL(Equals(cspan, span), true);

        SYNTROPY_UNIT_EQUAL(span == cspan, true);
        SYNTROPY_UNIT_EQUAL(span != cspan, false);

        SYNTROPY_UNIT_EQUAL(Equals(span, cspan), true);
    })

    .TestCase("Spans are equivalent to spans whose values compare equivalent.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto span_equivalent = RWSpan<Int>{ &fixture.int_sequence_alt_[0], 10 };
        auto span_different = RWSpan<Int>{ &fixture.int_sequence_[3], 7 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto cspan_equivalent = Span<Int>{ &fixture.int_sequence_alt_[0], 10 };
        auto cspan_different = Span<Int>{ &fixture.int_sequence_[3], 7 };

        SYNTROPY_UNIT_EQUAL(span == span_equivalent, true);
        SYNTROPY_UNIT_EQUAL(span != span_equivalent, false);
        SYNTROPY_UNIT_EQUAL(span == span_different, false);
        SYNTROPY_UNIT_EQUAL(span != span_different, true);

        SYNTROPY_UNIT_EQUAL(cspan == cspan_equivalent, true);
        SYNTROPY_UNIT_EQUAL(cspan != cspan_equivalent, false);
        SYNTROPY_UNIT_EQUAL(cspan == cspan_different, false);
        SYNTROPY_UNIT_EQUAL(cspan != cspan_different, true);

        SYNTROPY_UNIT_EQUAL(span == cspan_equivalent, true);
        SYNTROPY_UNIT_EQUAL(span != cspan_equivalent, false);
        SYNTROPY_UNIT_EQUAL(span == cspan_different, false);
        SYNTROPY_UNIT_EQUAL(span != cspan_different, true);

        SYNTROPY_UNIT_EQUAL(cspan == span_equivalent, true);
        SYNTROPY_UNIT_EQUAL(cspan != span_equivalent, false);
        SYNTROPY_UNIT_EQUAL(cspan == span_different, false);
        SYNTROPY_UNIT_EQUAL(cspan != span_different, true);
    })

    .TestCase("Spans are equal to spans with a different type if the elements are implicitly convertible and compare equal.", [](auto& fixture)
    {
        auto span_int = RWSpan<Int>{ &fixture.int_sequence_[0], 3 };
        auto span_float = RWSpan<Float>{ &fixture.float_sequence_[0], 3 };

        auto cspan_int = Span<Int>{ &fixture.int_sequence_[0], 3 };
        auto cspan_float = Span<Float>{ &fixture.float_sequence_[0], 3 };

        SYNTROPY_UNIT_EQUAL(Equals(span_int, span_float), true);
        SYNTROPY_UNIT_EQUAL(Equals(cspan_int, cspan_float), true);
        SYNTROPY_UNIT_EQUAL(Equals(span_int, cspan_float), true);
        SYNTROPY_UNIT_EQUAL(Equals(cspan_int, span_float), true);
    })

    .TestCase("Span front elements are readable.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Front(span), 0);
        SYNTROPY_UNIT_EQUAL(Front(cspan), 0);
    })

    .TestCase("Span front elements are writable.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };

        Front(span) = 42;

        SYNTROPY_UNIT_EQUAL(Front(span), 42);
    })

    .TestCase("Span back elements are readable.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Back(span), 9);
        SYNTROPY_UNIT_EQUAL(Back(cspan), 9);
    })

    .TestCase("Span back elements are writable.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };

        Back(span) = 42;

        SYNTROPY_UNIT_EQUAL(Back(span), 42);
    })

    .TestCase("Sub-spans that encompass the entire source span is equivalent to the latter.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Subspan(span, 0, Count(span)), span);
        SYNTROPY_UNIT_EQUAL(Subspan(cspan, 0, Count(cspan)), cspan);
    })

    .TestCase("Sub-spans with zero elements are empty.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Subspan(span, 0, 0), Span<Int>{});
        SYNTROPY_UNIT_EQUAL(Subspan(cspan, 0, 0), Span<const Int>{});
    })

    .TestCase("Sub-spans are equal to spans constructed from the same sequence.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto subspan = RWSpan<Int>{ &fixture.int_sequence_[3], 5 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto csubspan = Span<Int>{ &fixture.int_sequence_[3], 5 };

        SYNTROPY_UNIT_EQUAL(Subspan(span, 3, 5), subspan);
        SYNTROPY_UNIT_EQUAL(Subspan(cspan, 3, 5), csubspan);
    })

    .TestCase("Removing front elements from a span yields a sub-span which is equal to the remaining elements.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto popfront1 = RWSpan<Int>{ &fixture.int_sequence_[1], 9 };
        auto popfront3 = RWSpan<Int>{ &fixture.int_sequence_[3], 7 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto cpopfront1 = Span<Int>{ &fixture.int_sequence_[1], 9 };
        auto cpopfront3 = Span<Int>{ &fixture.int_sequence_[3], 7 };

        SYNTROPY_UNIT_EQUAL(PopFront(span), popfront1);
        SYNTROPY_UNIT_EQUAL(PopFront(span, 3), popfront3);

        SYNTROPY_UNIT_EQUAL(PopFront(cspan), cpopfront1);
        SYNTROPY_UNIT_EQUAL(PopFront(cspan, 3), cpopfront3);

        SYNTROPY_UNIT_EQUAL(PopFront(span), cpopfront1);
        SYNTROPY_UNIT_EQUAL(PopFront(span, 3), cpopfront3);

        SYNTROPY_UNIT_EQUAL(PopFront(cspan), popfront1);
        SYNTROPY_UNIT_EQUAL(PopFront(cspan, 3), popfront3);
    })

    .TestCase("Removing back elements from the span yields a sub-span which is equal to the remaining elements.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto popback1 = RWSpan<Int>{ &fixture.int_sequence_[0], 9 };
        auto popback3 = RWSpan<Int>{ &fixture.int_sequence_[0], 7 };

        auto cspan = Span< Int>{ &fixture.int_sequence_[0], 10 };
        auto cpopback1 = Span< Int>{ &fixture.int_sequence_[0], 9 };
        auto cpopback3 = Span< Int>{ &fixture.int_sequence_[0], 7 };

        SYNTROPY_UNIT_EQUAL(PopBack(span), popback1);
        SYNTROPY_UNIT_EQUAL(PopBack(span, 3), popback3);

        SYNTROPY_UNIT_EQUAL(PopBack(cspan), cpopback1);
        SYNTROPY_UNIT_EQUAL(PopBack(cspan, 3), cpopback3);

        SYNTROPY_UNIT_EQUAL(PopBack(span), cpopback1);
        SYNTROPY_UNIT_EQUAL(PopBack(span, 3), cpopback3);

        SYNTROPY_UNIT_EQUAL(PopBack(cspan), popback1);
        SYNTROPY_UNIT_EQUAL(PopBack(cspan, 3), popback3);
    })

    .TestCase("Selecting the first elements of a span yields a sub-span which has the selected elements only.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto first4 = RWSpan<Int>{ &fixture.int_sequence_[0], 4 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto cfirst4 = Span<Int>{ &fixture.int_sequence_[0], 4 };

        SYNTROPY_UNIT_EQUAL(First(span, 4), first4);
        SYNTROPY_UNIT_EQUAL(First(cspan, 4), cfirst4);
        SYNTROPY_UNIT_EQUAL(First(cspan, 4), first4);
        SYNTROPY_UNIT_EQUAL(First(span, 4), cfirst4);
    })

    .TestCase("Selecting the last elements of a span yields a sub-span which has the selected elements only.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto last4 = RWSpan<Int>{ &fixture.int_sequence_[6], 4 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto clast4 = Span<Int>{ &fixture.int_sequence_[6], 4 };

        SYNTROPY_UNIT_EQUAL(Last(span, 4), last4);
    })

    .TestCase("Spans have prefixes when they refer to a sequence whose elements compare equivalent.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };

        auto prefix_same = RWSpan<Int>{ &fixture.int_sequence_[0], 3 };
        auto prefix_equivalent = RWSpan<Int>{ &fixture.int_sequence_alt_[0], 3 };
        auto prefix_different = RWSpan<Int>{ &fixture.int_sequence_[2], 4 };
        auto prefix_convertible = RWSpan<Float>{ &fixture.float_sequence_[0], 3 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        auto cprefix_same = Span<Int>{ &fixture.int_sequence_[0], 3 };
        auto cprefix_equivalent = Span<Int>{ &fixture.int_sequence_alt_[0], 3 };
        auto cprefix_different = Span<Int>{ &fixture.int_sequence_[2], 4 };
        auto cprefix_convertible = Span<Float>{ &fixture.float_sequence_[0], 3 };

        SYNTROPY_UNIT_EQUAL(HasPrefix(span, prefix_same), true);
        SYNTROPY_UNIT_EQUAL(HasPrefix(span, prefix_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasPrefix(span, prefix_different), false);
        SYNTROPY_UNIT_EQUAL(HasPrefix(span, prefix_convertible), true);

        SYNTROPY_UNIT_EQUAL(HasPrefix(cspan, cprefix_same), true);
        SYNTROPY_UNIT_EQUAL(HasPrefix(cspan, cprefix_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasPrefix(cspan, cprefix_different), false);
        SYNTROPY_UNIT_EQUAL(HasPrefix(cspan, cprefix_convertible), true);

        SYNTROPY_UNIT_EQUAL(HasPrefix(cspan, prefix_same), true);
        SYNTROPY_UNIT_EQUAL(HasPrefix(cspan, prefix_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasPrefix(cspan, prefix_different), false);
        SYNTROPY_UNIT_EQUAL(HasPrefix(cspan, prefix_convertible), true);

        SYNTROPY_UNIT_EQUAL(HasPrefix(span, cprefix_same), true);
        SYNTROPY_UNIT_EQUAL(HasPrefix(span, cprefix_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasPrefix(span, cprefix_different), false);
        SYNTROPY_UNIT_EQUAL(HasPrefix(span, cprefix_convertible), true);
    })

    .TestCase("Spans have suffixes when they refer to a sequence whose elements compare equivalent.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };

        auto suffix_same = RWSpan<Int>{ &fixture.int_sequence_[7], 3 };
        auto suffix_equivalent = RWSpan<Int>{ &fixture.int_sequence_alt_[7], 3 };
        auto suffix_different = RWSpan<Int>{ &fixture.int_sequence_[1], 3 };
        auto suffix_convertible = RWSpan<Float>{ &fixture.float_sequence_[7], 3 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        auto csuffix_same = Span<Int>{ &fixture.int_sequence_[7], 3 };
        auto csuffix_equivalent = Span<Int>{ &fixture.int_sequence_alt_[7], 3 };
        auto csuffix_different = Span<Int>{ &fixture.int_sequence_[1], 3 };
        auto csuffix_convertible = Span<Float>{ &fixture.float_sequence_[7], 3 };

        SYNTROPY_UNIT_EQUAL(HasSuffix(span, suffix_same), true);
        SYNTROPY_UNIT_EQUAL(HasSuffix(span, suffix_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasSuffix(span, suffix_different), false);
        SYNTROPY_UNIT_EQUAL(HasSuffix(span, suffix_convertible), true);

        SYNTROPY_UNIT_EQUAL(HasSuffix(cspan, csuffix_same), true);
        SYNTROPY_UNIT_EQUAL(HasSuffix(cspan, csuffix_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasSuffix(cspan, csuffix_different), false);
        SYNTROPY_UNIT_EQUAL(HasSuffix(cspan, csuffix_convertible), true);

        SYNTROPY_UNIT_EQUAL(HasSuffix(span, csuffix_same), true);
        SYNTROPY_UNIT_EQUAL(HasSuffix(span, csuffix_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasSuffix(span, csuffix_different), false);
        SYNTROPY_UNIT_EQUAL(HasSuffix(span, csuffix_convertible), true);

        SYNTROPY_UNIT_EQUAL(HasSuffix(cspan, suffix_same), true);
        SYNTROPY_UNIT_EQUAL(HasSuffix(cspan, suffix_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasSuffix(cspan, suffix_different), false);
        SYNTROPY_UNIT_EQUAL(HasSuffix(cspan, suffix_convertible), true);
    })

    .TestCase("Spans have sub-spans when they refer to a sequence whose elements compare equivalent.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        auto span_same = RWSpan<Int>{ &fixture.int_sequence_[4], 3 };
        auto span_equivalent = RWSpan<Int>{ &fixture.int_sequence_alt_[4], 3 };
        auto span_different = RWSpan<Int>{ &fixture.const_sequence_[3], 2 };
        auto span_convertible = RWSpan<Float>{ &fixture.float_sequence_[4], 3 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        auto cspan_same = Span<Int>{ &fixture.int_sequence_[4], 3 };
        auto cspan_equivalent = Span<Int>{ &fixture.int_sequence_alt_[4], 3 };
        auto cspan_different = Span<Int>{ &fixture.const_sequence_[3], 2 };
        auto cspan_convertible = Span<Float>{ &fixture.float_sequence_[4], 3 };

        SYNTROPY_UNIT_EQUAL(HasSubspan(span, span_same), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(span, span_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(span, span_different), false);
        SYNTROPY_UNIT_EQUAL(HasSubspan(span, span_convertible), true);

        SYNTROPY_UNIT_EQUAL(HasSubspan(cspan, cspan_same), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(cspan, cspan_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(cspan, cspan_different), false);
        SYNTROPY_UNIT_EQUAL(HasSubspan(cspan, cspan_convertible), true);

        SYNTROPY_UNIT_EQUAL(HasSubspan(span, cspan_same), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(span, cspan_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(span, cspan_different), false);
        SYNTROPY_UNIT_EQUAL(HasSubspan(span, cspan_convertible), true);

        SYNTROPY_UNIT_EQUAL(HasSubspan(cspan, span_same), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(cspan, span_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(cspan, span_different), false);
        SYNTROPY_UNIT_EQUAL(HasSubspan(cspan, span_convertible), true);
    })

    .TestCase("Empty spans are contained in any other span.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };

        auto empty_int = RWSpan<Int>{};
        auto empty_float = RWSpan<Float>{};

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        auto cempty_int = Span<Int>{};
        auto cempty_float = Span<Float>{};

        SYNTROPY_UNIT_EQUAL(HasSubspan(span, empty_int), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(span, empty_float), true);

        SYNTROPY_UNIT_EQUAL(HasSubspan(cspan, cempty_int), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(cspan, cempty_float), true);

        SYNTROPY_UNIT_EQUAL(HasSubspan(span, cempty_int), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(span, cempty_float), true);

        SYNTROPY_UNIT_EQUAL(HasSubspan(cspan, empty_int), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(cspan, empty_float), true);
    })

    .TestCase("Searching for subspan returns a sequence which has that subspan as prefix.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };

        auto span_same = RWSpan<Int>{ &fixture.int_sequence_[4], 3 };
        auto span_equivalent = RWSpan<Int>{ &fixture.int_sequence_alt_[4], 3 };
        auto span_different = RWSpan<Int>{ &fixture.const_sequence_[3], 2 };
        auto span_convertible = RWSpan<Float>{ &fixture.float_sequence_[4], 3 };

        auto search_same = RWSpan<Int>{ &fixture.int_sequence_[4], 6 };
        auto search_equivalent = RWSpan<Int>{ &fixture.int_sequence_[4], 6 };
        auto search_different = RWSpan<Int>{};
        auto search_convertible = RWSpan<Int>{ &fixture.int_sequence_[4], 6 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        auto cspan_same = Span<Int>{ &fixture.int_sequence_[4], 3 };
        auto cspan_equivalent = Span<Int>{ &fixture.int_sequence_alt_[4], 3 };
        auto cspan_different = Span<Int>{ &fixture.const_sequence_[3], 2 };
        auto cspan_convertible = Span<Float>{ &fixture.float_sequence_[4], 3 };

        auto csearch_same = Span<Int>{ &fixture.int_sequence_[4], 6 };
        auto csearch_equivalent = Span<Int>{ &fixture.int_sequence_[4], 6 };
        auto csearch_different = Span<Int>{};
        auto csearch_convertible = Span<Int>{ &fixture.int_sequence_[4], 6 };

        SYNTROPY_UNIT_EQUAL(Search(span, span_same), search_same);
        SYNTROPY_UNIT_EQUAL(Search(span, span_equivalent), search_equivalent);
        SYNTROPY_UNIT_EQUAL(Search(span, span_different), search_different);
        SYNTROPY_UNIT_EQUAL(Search(span, span_convertible), search_convertible);

        SYNTROPY_UNIT_EQUAL(Search(cspan, cspan_same), csearch_same);
        SYNTROPY_UNIT_EQUAL(Search(cspan, cspan_equivalent), csearch_equivalent);
        SYNTROPY_UNIT_EQUAL(Search(cspan, cspan_different), csearch_different);
        SYNTROPY_UNIT_EQUAL(Search(cspan, cspan_convertible), csearch_convertible);
    })

    .TestCase("Searching for a subspan returns an empty sequence if the search fails.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };

        auto span_different = RWSpan<Int>{ &fixture.const_sequence_[3], 2 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        auto cspan_different = Span<Int>{ &fixture.const_sequence_[3], 2 };

        SYNTROPY_UNIT_EQUAL(Search(span, span_different), Span<Int>{});
        SYNTROPY_UNIT_EQUAL(Search(cspan, cspan_different), Span<Int>{});
        SYNTROPY_UNIT_EQUAL(Search(span, cspan_different), Span<Int>{});
        SYNTROPY_UNIT_EQUAL(Search(cspan, span_different), Span<Int>{});
    })

    .TestCase("Searching for an empty span in another span returns the original span.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Search(span, RWSpan<Int>{}), span);

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Search(span, RWSpan<Int>{}), span);
        SYNTROPY_UNIT_EQUAL(Search(span, Span<Int>{}), cspan);
    })

    .TestCase("Span contain themselves.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 4 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 4 };

        SYNTROPY_UNIT_EQUAL(Contains(span, span), true);
        SYNTROPY_UNIT_EQUAL(Contains(cspan, cspan), true);
    })

    .TestCase("Span contains a subspan if the latter refers to a memory location inside the first.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto subspan = RWSpan<Int>{ &fixture.int_sequence_[2], 4 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto csubspan = Span<Int>{ &fixture.int_sequence_[2], 4 };

        SYNTROPY_UNIT_EQUAL(Contains(span, subspan), true);
        SYNTROPY_UNIT_EQUAL(Contains(cspan, csubspan), true);
    })

    .TestCase("Empty spans are contained in any other span.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 10 };
        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Contains(span, RWSpan<Int>{}), true);
        SYNTROPY_UNIT_EQUAL(Contains(cspan, Span<Int>{}), true);
        SYNTROPY_UNIT_EQUAL(Contains(cspan, RWSpan<Int>{}), true);
        SYNTROPY_UNIT_EQUAL(Contains(span, Span<Int>{}), true);
    })

    .TestCase("Empty spans are not contained in themselves.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Contains(RWSpan<Int>{}, RWSpan<Int>{}), false);
        SYNTROPY_UNIT_EQUAL(Contains(Span<Int>{}, Span<Int>{}), false);
        SYNTROPY_UNIT_EQUAL(Contains(RWSpan<Int>{}, Span<Int>{}), false);
        SYNTROPY_UNIT_EQUAL(Contains(Span<Int>{}, RWSpan<Int>{}), false);
    })

    .TestCase("Span overlap with themselves.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 4 };
        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 4 };

        SYNTROPY_UNIT_EQUAL(Overlaps(span, span), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(cspan, cspan), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(span, cspan), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(cspan, span), true);
    })

    .TestCase("Disjoint spans do not overlap.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 4 };
        auto disjoint = RWSpan<Int>{ &fixture.int_sequence_[6], 4 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 4 };
        auto cdisjoint = Span<Int>{ &fixture.int_sequence_[6], 4 };

        SYNTROPY_UNIT_EQUAL(Overlaps(span, disjoint), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(cspan, cdisjoint), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(cspan, disjoint), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(span, cdisjoint), false);
    })

    .TestCase("Contiguous spans do not overlap.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 4 };
        auto contiguous = RWSpan<Int>{ &fixture.int_sequence_[4], 3 };

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 4 };
        auto ccontiguous = Span<Int>{ &fixture.int_sequence_[4], 3 };

        SYNTROPY_UNIT_EQUAL(Overlaps(span, contiguous), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(cspan, ccontiguous), false);
    })

    .TestCase("Empty spans do not overlap with any other span.", [](auto& fixture)
    {
        auto span = RWSpan<Int>{ &fixture.int_sequence_[0], 4 };
        auto empty = RWSpan<Int>{};

        auto cspan = Span<Int>{ &fixture.int_sequence_[0], 4 };
        auto cempty = Span<Int>{};

        SYNTROPY_UNIT_EQUAL(Overlaps(empty, empty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(span, empty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(empty, span), false);

        SYNTROPY_UNIT_EQUAL(Overlaps(cempty, cempty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(cspan, cempty), false);
        SYNTROPY_UNIT_EQUAL(Overlaps(cempty, cspan), false);
    })

    .TestCase("Overlapping test is commutative.", [](auto& fixture)
    {
        auto left = RWSpan<Int>{ &fixture.int_sequence_[0], 4 };
        auto right = RWSpan<Int>{ &fixture.int_sequence_[2], 4 };

        auto cleft = Span<Int>{ &fixture.int_sequence_[0], 4 };
        auto cright = Span<Int>{ &fixture.int_sequence_[2], 4 };

        SYNTROPY_UNIT_EQUAL(Overlaps(left, right), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(right, left), true);

        SYNTROPY_UNIT_EQUAL(Overlaps(cleft, cright), true);
        SYNTROPY_UNIT_EQUAL(Overlaps(cright, cleft), true);
    });

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