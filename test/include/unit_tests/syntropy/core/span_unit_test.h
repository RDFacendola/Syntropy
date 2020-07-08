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

    inline const auto& span_unit_test = MakeAutoUnitTest<SpanTestFixture>("Span")

    .TestCase("Default constructed spans are empty.", [](auto& fixture)
    {
        auto span = Span<Int>{};

        SYNTROPY_UNIT_EQUAL(!span, true);
        SYNTROPY_UNIT_EQUAL(span.GetCount(), 0);
        SYNTROPY_UNIT_EQUAL(span.GetData(), nullptr);
    })

    .TestCase("Spans constructed from an iterator and a non-zero number of elements are non-empty.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(!!span, true);
        SYNTROPY_UNIT_EQUAL(span.GetCount(), 10);
        SYNTROPY_UNIT_EQUAL(span.GetData(), &fixture.int_sequence_[0]);
    })

    .TestCase("Spans constructed from a pair of non-equal iterator are non-empty.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], &fixture.int_sequence_[0] + 10 };

        SYNTROPY_UNIT_EQUAL(!!span, true);
        SYNTROPY_UNIT_EQUAL(span.GetCount(), 10);
        SYNTROPY_UNIT_EQUAL(span.GetData(), &fixture.int_sequence_[0]);
    })

    .TestCase("Spans provide read-only access to elements.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(span[5], 5);
    })

    .TestCase("Non-constant spans provide read-write access to elements.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        
        span[5] = 42;

        SYNTROPY_UNIT_EQUAL(span[5], 42);
    })

    .TestCase("Spans are always equivalent to themselves.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(span == span, true);
        SYNTROPY_UNIT_EQUAL(span != span, false);

        SYNTROPY_UNIT_EQUAL(AreEquivalent(span, span), true);
    })

    .TestCase("Spans are equivalent to spans whose values compare equivalent.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span_equivalent = Span<Int>{ &fixture.int_sequence_alt_[0], 10 };
        auto span_different = Span<Int>{ &fixture.int_sequence_[3], 7 };

        SYNTROPY_UNIT_EQUAL(span == span_equivalent, true);
        SYNTROPY_UNIT_EQUAL(span != span_equivalent, false);
        SYNTROPY_UNIT_EQUAL(span == span_different, false);
        SYNTROPY_UNIT_EQUAL(span != span_different, true);
    })

    .TestCase("Spans are equal to spans with a different type if the elements are implicitly convertible and compare equal.", [](auto& fixture)
    {
        auto span_int = Span<Int>{ &fixture.int_sequence_[0], 3 };
        auto span_float = Span<Float>{ &fixture.float_sequence_[0], 3 };

        SYNTROPY_UNIT_EQUAL(span_int == span_float, true);
    })

    .TestCase("Spans are not identical to other spans, even if they compare equivalent.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span_equivalent = Span<Int>{ &fixture.int_sequence_alt_[0], 10 };
        auto span_identical = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(AreIdentical(span, span_equivalent), false);
        SYNTROPY_UNIT_EQUAL(AreIdentical(span, span_identical), true);
    })

    .TestCase("Span front elements are readable.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Front(span), 0);
    })

    .TestCase("Span front elements are writable.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        Front(span) = 42;

        SYNTROPY_UNIT_EQUAL(Front(span), 42);
    })

    .TestCase("Span back elements are readable.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Back(span), 9);
    })

    .TestCase("Span back elements are writable.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        Back(span) = 42;

        SYNTROPY_UNIT_EQUAL(Back(span), 42);
    })

    .TestCase("Sub-spans that encompass the entire source span is equivalent to the latter.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Subspan(span, 0, Count(span)), span);
    })

    .TestCase("Sub-spans with zero elements are empty.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Subspan(span, 0, 0), Span<Int>{});
    })

    .TestCase("Sub-spans are equal to spans constructed from the same sequence.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto subspan = Span<Int>{ &fixture.int_sequence_[3], 5 };

        SYNTROPY_UNIT_EQUAL(Subspan(span, 3, 5), subspan);
    })

    .TestCase("Removing front elements from a span yields a sub-span which is equal to the remaining elements.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto popfront1 = Span<Int>{ &fixture.int_sequence_[1], 9 };
        auto popfront3 = Span<Int>{ &fixture.int_sequence_[3], 7 };

        SYNTROPY_UNIT_EQUAL(PopFront(span), popfront1);
        SYNTROPY_UNIT_EQUAL(PopFront(span, 3), popfront3);
    })

    .TestCase("Removing back elements from the span yields a sub-span which is equal to the remaining elements.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto popback1 = Span<Int>{ &fixture.int_sequence_[0], 9 };
        auto popback3 = Span<Int>{ &fixture.int_sequence_[0], 7 };

        SYNTROPY_UNIT_EQUAL(PopBack(span), popback1);
        SYNTROPY_UNIT_EQUAL(PopBack(span, 3), popback3);
    })

    .TestCase("Selecting the first elements of a span yields a sub-span which has the selected elements only.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto first4 = Span<Int>{ &fixture.int_sequence_[0], 4 };

        SYNTROPY_UNIT_EQUAL(First(span, 4), first4);
    })

    .TestCase("Selecting the last elements of a span yields a sub-span which has the selected elements only.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto last4 = Span<Int>{ &fixture.int_sequence_[6], 4 };

        SYNTROPY_UNIT_EQUAL(Last(span, 4), last4);
    })

    .TestCase("Spans have prefixes when they refer to a sequence whose elements compare equivalent.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        auto prefix_same = Span<Int>{ &fixture.int_sequence_[0], 3 };
        auto prefix_equivalent = Span<Int>{ &fixture.int_sequence_alt_[0], 3 };
        auto prefix_different = Span<Int>{ &fixture.int_sequence_[2], 4 };
        auto prefix_convertible = Span<Float>{ &fixture.float_sequence_[0], 3 };

        SYNTROPY_UNIT_EQUAL(HasPrefix(span, prefix_same), true);
        SYNTROPY_UNIT_EQUAL(HasPrefix(span, prefix_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasPrefix(span, prefix_different), false);
        SYNTROPY_UNIT_EQUAL(HasPrefix(span, prefix_convertible), true);
    })

    .TestCase("Spans have suffixes when they refer to a sequence whose elements compare equivalent.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        auto suffix_same = Span<Int>{ &fixture.int_sequence_[7], 3 };
        auto suffix_equivalent = Span<Int>{ &fixture.int_sequence_alt_[7], 3 };
        auto suffix_different = Span<Int>{ &fixture.int_sequence_[1], 3 };
        auto suffix_convertible = Span<Float>{ &fixture.float_sequence_[7], 3 };

        SYNTROPY_UNIT_EQUAL(HasSuffix(span, suffix_same), true);
        SYNTROPY_UNIT_EQUAL(HasSuffix(span, suffix_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasSuffix(span, suffix_different), false);
        SYNTROPY_UNIT_EQUAL(HasSuffix(span, suffix_convertible), true);
    })

    .TestCase("Spans have sub-spans when they refer to a sequence whose elements compare equivalent.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        auto span_same = Span<Int>{ &fixture.int_sequence_[4], 3 };
        auto span_equivalent = Span<Int>{ &fixture.int_sequence_alt_[4], 3 };
        auto span_different = Span<Int>{ &fixture.const_sequence_[3], 2 };
        auto span_convertible = Span<Float>{ &fixture.float_sequence_[4], 3 };

        SYNTROPY_UNIT_EQUAL(HasSubspan(span, span_same), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(span, span_equivalent), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(span, span_different), false);
        SYNTROPY_UNIT_EQUAL(HasSubspan(span, span_convertible), true);
    })

    .TestCase("Empty spans are contained in any other span.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        auto empty_int = Span<Int>{};
        auto empty_float = Span<Float>{};

        SYNTROPY_UNIT_EQUAL(HasSubspan(span, empty_int), true);
        SYNTROPY_UNIT_EQUAL(HasSubspan(span, empty_float), true);
    })

    .TestCase("Searching for subspan returns a sequence which has that subspan as prefix.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        auto span_same = Span<Int>{ &fixture.int_sequence_[4], 3 };
        auto span_equivalent = Span<Int>{ &fixture.int_sequence_alt_[4], 3 };
        auto span_different = Span<Int>{ &fixture.const_sequence_[3], 2 };
        auto span_convertible = Span<Float>{ &fixture.float_sequence_[4], 3 };

        auto search_same = Span<Int>{ &fixture.int_sequence_[4], 6 };
        auto search_equivalent = Span<Int>{ &fixture.int_sequence_[4], 6 };
        auto search_different = Span<Int>{};
        auto search_convertible = Span<Int>{ &fixture.int_sequence_[4], 6 };

        SYNTROPY_UNIT_EQUAL(Search(span, span_same), search_same);
        SYNTROPY_UNIT_EQUAL(Search(span, span_equivalent), search_equivalent);
        SYNTROPY_UNIT_EQUAL(Search(span, span_different), search_different);
        SYNTROPY_UNIT_EQUAL(Search(span, span_convertible), search_convertible);
    })

    .TestCase("Searching for a subspan returns an empty sequence if the search fails.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        auto span_different = Span<Int>{ &fixture.const_sequence_[3], 2 };

        SYNTROPY_UNIT_EQUAL(Search(span, span_different), Span<Int>{});
    })

    .TestCase("Searching for an empty span in another span returns the original span.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Search(span, Span<Int>{}), span);
    })

    .TestCase("Span contain themselves.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 4 };

        SYNTROPY_UNIT_EQUAL(Contains(span, span), true);
    })

    .TestCase("Span contains a subspan if the first refers to a memory location inside the first.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto subspan = Span<Int>{ &fixture.int_sequence_[2], 4 };

        SYNTROPY_UNIT_EQUAL(Contains(span, subspan), true);
    })

    .TestCase("Empty spans are contained in any other span.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Contains(span, Span<Int>{}), true);
    })

    .TestCase("Span overlap with themselves.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 4 };

        SYNTROPY_UNIT_EQUAL(IsOverlapping(span, span), true);
    })

    .TestCase("Disjoint spans do not overlap.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 4 };
        auto disjoint = Span<Int>{ &fixture.int_sequence_[6], 4 };

        SYNTROPY_UNIT_EQUAL(IsOverlapping(span, disjoint), false);
    })

    .TestCase("Contiguous spans do not overlap.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 4 };
        auto contiguous = Span<Int>{ &fixture.int_sequence_[4], 3 };

        SYNTROPY_UNIT_EQUAL(IsOverlapping(span, contiguous), false);
    })

    .TestCase("Empty spans do not overlap with any other span.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 4 };
        auto empty = Span<Int>{};

        SYNTROPY_UNIT_EQUAL(IsOverlapping(empty, empty), false);
        SYNTROPY_UNIT_EQUAL(IsOverlapping(span, empty), false);
        SYNTROPY_UNIT_EQUAL(IsOverlapping(empty, span), false);
    })

    .TestCase("Overlapping test is commutative.", [](auto& fixture)
    {
        auto left = Span<Int>{ &fixture.int_sequence_[0], 4 };
        auto right = Span<Int>{ &fixture.int_sequence_[2], 4 };

        SYNTROPY_UNIT_EQUAL(IsOverlapping(left, right), true);
        SYNTROPY_UNIT_EQUAL(IsOverlapping(right, left), true);
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