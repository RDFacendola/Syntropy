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
        auto span_default = Span<Int>{};
        auto empty = Span<Int>{ &fixture.int_sequence_[0], 0 };

        SYNTROPY_UNIT_EQUAL(span_default.GetCount(), 0);
        SYNTROPY_UNIT_EQUAL(Count(span_default), 0);
        SYNTROPY_UNIT_EQUAL(!span_default, true);
        SYNTROPY_UNIT_EQUAL(IsEmpty(span_default), true);

        SYNTROPY_UNIT_EQUAL(empty.GetCount(), 0);
        SYNTROPY_UNIT_EQUAL(Count(empty), 0);
        SYNTROPY_UNIT_EQUAL(!empty, true);
        SYNTROPY_UNIT_EQUAL(IsEmpty(empty), true);
    })

    .TestCase("Spans constructed from a pair of non-equal iterator are non-empty.", [](auto& fixture)
    {
        auto nempty = Span<Int>{ &fixture.int_sequence_[0], &fixture.int_sequence_[9] + 1 };

        SYNTROPY_UNIT_EQUAL(nempty.GetCount(), 10);
        SYNTROPY_UNIT_EQUAL(Count(nempty), 10);
        SYNTROPY_UNIT_EQUAL(!!nempty, true);
        SYNTROPY_UNIT_EQUAL(IsEmpty(nempty), false);
    })

    .TestCase("Spans constructed from an iterator and a non-zero number of elements are non-empty.", [](auto& fixture)
    {
        auto nempty = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(nempty.GetCount(), 10);
        SYNTROPY_UNIT_EQUAL(!!nempty, true);
        SYNTROPY_UNIT_EQUAL(IsEmpty(nempty), false);
    })

    .TestCase("Spans provide random read-only access to both const and non-const elements.", [](auto& fixture)
    {
        auto span_c = Span<const Int>{ &fixture.int_sequence_[0], 10 };
        auto span_nc = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(span_c[5], 5);
        SYNTROPY_UNIT_EQUAL(span_nc[4], 4);
    })

    .TestCase("Spans provide random read-write access to non-const elements.", [](auto& fixture)
    {
        auto span_nc = Span<Int>{ &fixture.int_sequence_[0], 10 };

        span_nc[5] = 42;

        SYNTROPY_UNIT_EQUAL(span_nc[5], 42);
    })

    .TestCase("Spans are always equal to themselves.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(span == span, true);
        SYNTROPY_UNIT_EQUAL(span != span, false);
    })

    .TestCase("Spans are equal only to other spans containing the same elements in the same order.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 9 };
        auto span_id = Span<Int>{ &fixture.int_sequence_[0], 9 };
        auto span_neq = Span<Int>{ &fixture.int_sequence_[3], 7 };

        SYNTROPY_UNIT_EQUAL(span == span_id, true);
        SYNTROPY_UNIT_EQUAL(span != span_id, false);
        SYNTROPY_UNIT_EQUAL(span == span_neq, false);
        SYNTROPY_UNIT_EQUAL(span != span_neq, true);
    })

     .TestCase("Spans are equal to other spans with a different type if the elements are implicitly convertible and compare equal.", [](auto& fixture)
    {
        auto span_int = Span<Int>{ &fixture.int_sequence_[0], 3 };
        auto span_float = Span<Float>{ &fixture.float_sequence_[0], 3 };

        SYNTROPY_UNIT_EQUAL(span_int == span_float, true);
    })

    .TestCase("Span front elements are both readable and writable.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Front(span), 0);

        Front(span) = 42;

        SYNTROPY_UNIT_EQUAL(Front(span), 42);
    })

    .TestCase("Back elements are both readable and writable.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Back(span), 9);

        Back(span) = 42;

        SYNTROPY_UNIT_EQUAL(Back(span), 42);
    })

    .TestCase("Sub-spans that encompass the entire source span are equal to the latter.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Subspan(span, 0, span.GetCount()), span);
    })

    .TestCase("Sub-spans with zero elements are empty.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto empty = Span<Int>{};

        SYNTROPY_UNIT_EQUAL(Subspan(span, 0, 0), empty);
    })

    .TestCase("Sub-spans are equal to spans constructed with the same elements.", [](auto& fixture)
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
        auto first = Span<Int>{ &fixture.int_sequence_[0], 4 };

        SYNTROPY_UNIT_EQUAL(First(span, 4), first);
    })

    .TestCase("Selecting the last elements of a span yields a sub-span which has the selected elements only.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto last = Span<Int>{ &fixture.int_sequence_[6], 4 };

        SYNTROPY_UNIT_EQUAL(Last(span, 4), last);
    })

    .TestCase("Spans are strongly equal to themselves.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(EqualsStrong(span, span), true);
    })

    .TestCase("Spans are weakly equal to themselves.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(EqualsWeak(span, span), true);
    })

    .TestCase("Spans are strongly equal to spans referring to the same memory location.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span_id = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span_eq = Span<Int>{ &fixture.int_sequence_alt_[0], 10 };
        auto span_neq = Span<Int>{ &fixture.int_sequence_[2], 8 };

        SYNTROPY_UNIT_EQUAL(EqualsStrong(span, span_id), true);
        SYNTROPY_UNIT_EQUAL(EqualsStrong(span, span_eq), false);
        SYNTROPY_UNIT_EQUAL(EqualsStrong(span, span_neq), false);
    })
    
    .TestCase("Spans are weakly equal to spans whose values compares equivalent.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span_id = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span_eq = Span<Int>{ &fixture.int_sequence_alt_[0], 10 };
        auto span_neq = Span<Int>{ &fixture.int_sequence_[2], 8 };
        auto span_feq = Span<Float>{ &fixture.float_sequence_[0], 10 };

        SYNTROPY_UNIT_EQUAL(EqualsWeak(span, span_id), true);
        SYNTROPY_UNIT_EQUAL(EqualsWeak(span, span_eq), true);
        SYNTROPY_UNIT_EQUAL(EqualsWeak(span, span_neq), false);
        SYNTROPY_UNIT_EQUAL(EqualsWeak(span, span_feq), true);
    })

    .TestCase("Empty spans are all strongly and weakly equivalent.", [](auto& fixture)
    {
        auto empty_1 = Span<Int>{};
        auto empty_2 = Span<Int>{ &fixture.int_sequence_[0], 0 };
        auto empty_f = Span<Float>{};

        SYNTROPY_UNIT_EQUAL(EqualsStrong(empty_1, empty_2), true);
        SYNTROPY_UNIT_EQUAL(EqualsStrong(empty_2, empty_1), true);
        SYNTROPY_UNIT_EQUAL(EqualsWeak(empty_1, empty_2), true);
        SYNTROPY_UNIT_EQUAL(EqualsWeak(empty_2, empty_1), true);
        SYNTROPY_UNIT_EQUAL(EqualsWeak(empty_1, empty_f), true);
        SYNTROPY_UNIT_EQUAL(EqualsWeak(empty_f, empty_1), true);
    })

    .TestCase("Spans have strong prefixes when they refer to the same memory region.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto prefix_id = Span<Int>{ &fixture.int_sequence_[0], 3 };
        auto prefix_eq = Span<Int>{ &fixture.int_sequence_alt_[0], 3 };
        auto prefix_neq = Span<Int>{ &fixture.int_sequence_[1], 3 };

        SYNTROPY_UNIT_EQUAL(HasPrefixStrong(span, prefix_id), true);
        SYNTROPY_UNIT_EQUAL(HasPrefixStrong(span, prefix_eq), false);
        SYNTROPY_UNIT_EQUAL(HasPrefixStrong(span, prefix_neq), false);
    })

    .TestCase("Spans have weak prefixes when they refer to a sequence whose elements compare equivalent.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto prefix_id = Span<Int>{ &fixture.int_sequence_[0], 3 };
        auto prefix_eq = Span<Int>{ &fixture.int_sequence_alt_[0], 3 };
        auto prefix_neq = Span<Int>{ &fixture.int_sequence_[2], 4 };
        auto prefix_feq = Span<Float>{ &fixture.float_sequence_[0], 3 };

        SYNTROPY_UNIT_EQUAL(HasPrefixWeak(span, prefix_id), true);
        SYNTROPY_UNIT_EQUAL(HasPrefixWeak(span, prefix_eq), true);
        SYNTROPY_UNIT_EQUAL(HasPrefixWeak(span, prefix_neq), false);
        SYNTROPY_UNIT_EQUAL(HasPrefixWeak(span, prefix_feq), true);
    })

    .TestCase("Empty spans are strong and weak prefix of any other span.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto empty = Span<Int>{};
        auto empty_f = Span<Float>{};

        SYNTROPY_UNIT_EQUAL(HasPrefixStrong(span, empty), true);
        SYNTROPY_UNIT_EQUAL(HasPrefixStrong(empty, empty), true);
        SYNTROPY_UNIT_EQUAL(HasPrefixWeak(span, empty), true);
        SYNTROPY_UNIT_EQUAL(HasPrefixWeak(span, empty_f), true);
        SYNTROPY_UNIT_EQUAL(HasPrefixWeak(empty, empty_f), true);
        SYNTROPY_UNIT_EQUAL(HasPrefixWeak(empty_f, empty), true);
    })

    .TestCase("Spans have strong suffixes when they refer to the same memory region.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto suffix_id = Span<Int>{ &fixture.int_sequence_[7], 3 };
        auto suffix_eq = Span<Int>{ &fixture.int_sequence_alt_[7], 3 };
        auto suffix_neq = Span<Int>{ &fixture.int_sequence_[2], 5 };

        SYNTROPY_UNIT_EQUAL(HasSuffixStrong(span, suffix_id), true);
        SYNTROPY_UNIT_EQUAL(HasSuffixStrong(span, suffix_eq), false);
        SYNTROPY_UNIT_EQUAL(HasSuffixStrong(span, suffix_neq), false);
    })

    .TestCase("Spans have weak suffixes when they refer to a sequence whose elements compare equivalent.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto suffix_id = Span<Int>{ &fixture.int_sequence_[7], 3 };
        auto suffix_eq = Span<Int>{ &fixture.int_sequence_alt_[7], 3 };
        auto suffix_neq = Span<Int>{ &fixture.int_sequence_[1], 3 };
        auto suffix_feq = Span<Float>{ &fixture.float_sequence_[7], 3 };

        SYNTROPY_UNIT_EQUAL(HasSuffixWeak(span, suffix_id), true);
        SYNTROPY_UNIT_EQUAL(HasSuffixWeak(span, suffix_eq), true);
        SYNTROPY_UNIT_EQUAL(HasSuffixWeak(span, suffix_neq), false);
        SYNTROPY_UNIT_EQUAL(HasSuffixWeak(span, suffix_feq), true);
    })

    .TestCase("Empty spans are strong and weak suffix of any other span.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto empty = Span<Int>{};
        auto empty_f = Span<Float>{};

        SYNTROPY_UNIT_EQUAL(HasSuffixStrong(span, empty), true);
        SYNTROPY_UNIT_EQUAL(HasSuffixStrong(empty, empty), true);
        SYNTROPY_UNIT_EQUAL(HasSuffixWeak(span, empty), true);
        SYNTROPY_UNIT_EQUAL(HasSuffixWeak(span, empty_f), true);
        SYNTROPY_UNIT_EQUAL(HasSuffixWeak(empty, empty_f), true);
        SYNTROPY_UNIT_EQUAL(HasSuffixWeak(empty_f, empty), true);
    })

    .TestCase("Spans have strong sub-spans when they refer to the same memory region.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span_id = Span<Int>{ &fixture.int_sequence_[4], 3 };
        auto span_eq = Span<Int>{ &fixture.int_sequence_alt_[4], 3 };
        auto span_neq = Span<Int>{ &fixture.const_sequence_[3], 2 };

        SYNTROPY_UNIT_EQUAL(ContainsStrong(span, span_id), true);
        SYNTROPY_UNIT_EQUAL(ContainsStrong(span, span_eq), false);
        SYNTROPY_UNIT_EQUAL(ContainsStrong(span, span_neq), false);
    })

    .TestCase("Spans have weak sub-spans when they refer to a sequence whose elements compare equivalent.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span_id = Span<Int>{ &fixture.int_sequence_[4], 3 };
        auto span_eq = Span<Int>{ &fixture.int_sequence_alt_[4], 3 };
        auto span_neq = Span<Int>{ &fixture.const_sequence_[3], 2 };
        auto span_f = Span<Float>{ &fixture.float_sequence_[4], 3 };

        SYNTROPY_UNIT_EQUAL(ContainsWeak(span, span_id), true);
        SYNTROPY_UNIT_EQUAL(ContainsWeak(span, span_eq), true);
        SYNTROPY_UNIT_EQUAL(ContainsWeak(span, span_neq), false);
        SYNTROPY_UNIT_EQUAL(ContainsWeak(span, span_f), true);
    })

    .TestCase("Empty spans are strongly and weakly contained in any other span.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto empty = Span<Int>{};
        auto empty_f = Span<Float>{};

        SYNTROPY_UNIT_EQUAL(ContainsStrong(span, empty), true);
        SYNTROPY_UNIT_EQUAL(ContainsStrong(empty, empty), true);
        SYNTROPY_UNIT_EQUAL(ContainsWeak(span, empty), true);
        SYNTROPY_UNIT_EQUAL(ContainsWeak(span, empty_f), true);
        SYNTROPY_UNIT_EQUAL(ContainsWeak(empty, empty_f), true);
        SYNTROPY_UNIT_EQUAL(ContainsWeak(empty_f, empty), true);
    })

    .TestCase("Spans have strong sub-spans when they refer to the same memory region.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span_id = Span<Int>{ &fixture.int_sequence_[4], 3 };
        auto span_eq = Span<Int>{ &fixture.int_sequence_alt_[4], 3 };
        auto span_neq = Span<Int>{ &fixture.const_sequence_[3], 2 };

        auto search_id = Span<Int>{ &fixture.int_sequence_[4], 6 };
        auto search_eq = Span<Int>{};
        auto search_neq = Span<Int>{};

        SYNTROPY_UNIT_EQUAL(SearchStrong(span, span_id), search_id);
        SYNTROPY_UNIT_EQUAL(SearchStrong(span, span_eq), search_eq);
        SYNTROPY_UNIT_EQUAL(SearchStrong(span, span_neq), search_neq);
    })

    .TestCase("Spans have weak sub-spans when they refer to a sequence whose elements compare equivalent.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto span_id = Span<Int>{ &fixture.int_sequence_[4], 3 };
        auto span_eq = Span<Int>{ &fixture.int_sequence_alt_[4], 3 };
        auto span_neq = Span<Int>{ &fixture.const_sequence_[3], 2 };
        auto span_f = Span<Float>{ &fixture.float_sequence_[4], 3 };

        auto search_id = Span<Int>{ &fixture.int_sequence_[4], 6 };
        auto search_eq = Span<Int>{ &fixture.int_sequence_[4], 6 };
        auto search_neq = Span<Int>{};
        auto search_f = Span<Int>{ &fixture.int_sequence_[4], 6 };

        SYNTROPY_UNIT_EQUAL(SearchWeak(span, span_id), search_id);
        SYNTROPY_UNIT_EQUAL(SearchWeak(span, span_eq), search_eq);
        SYNTROPY_UNIT_EQUAL(SearchWeak(span, span_neq), search_neq);
        SYNTROPY_UNIT_EQUAL(SearchWeak(span, span_f), search_f);
    })

    .TestCase("Searching for an empty span in another span returns the latter.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 10 };
        auto empty = Span<Int>{};

        SYNTROPY_UNIT_EQUAL(SearchStrong(span, empty), span);
        SYNTROPY_UNIT_EQUAL(SearchWeak(span, empty), span);
    })

    .TestCase("Span overlap with themselves.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 4 };

        SYNTROPY_UNIT_EQUAL(IsOverlapping(span, span), true);
    })

    .TestCase("Disjoint and contiguous spans do not overlap.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 4 };
        auto disjoint = Span<Int>{ &fixture.int_sequence_[6], 4 };
        auto contiguous = Span<Int>{ &fixture.int_sequence_[4], 3 };

        SYNTROPY_UNIT_EQUAL(IsOverlapping(span, disjoint), false);
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
    })

    .TestCase("A span is not contiguous to itself.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 4 };

        SYNTROPY_UNIT_EQUAL(IsContiguous(span, span), false);
    })

    .TestCase("Empty spans are not contiguous to any other span.", [](auto& fixture)
    {
        auto span = Span<Int>{ &fixture.int_sequence_[0], 4 };
        auto empty = Span<Int>{};

        SYNTROPY_UNIT_EQUAL(IsContiguous(empty, empty), false);
        SYNTROPY_UNIT_EQUAL(IsContiguous(span, empty), false);
        SYNTROPY_UNIT_EQUAL(IsContiguous(empty, span), false);
    })

    .TestCase("Contiguous relationship is antisymmetric.", [](auto& fixture)
    {
        auto left = Span<Int>{ &fixture.int_sequence_[0], 4 };
        auto right = Span<Int>{ &fixture.int_sequence_[4], 4 };

        SYNTROPY_UNIT_EQUAL(IsContiguous(left, right), true);
        SYNTROPY_UNIT_EQUAL(IsContiguous(right, left), false);
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