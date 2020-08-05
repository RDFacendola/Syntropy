#/// \file span_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/core/array.h"
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
        Array<Int, 10> ints_ = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        /// \brief Integer sequence.
        Array<Int, 10> ints_a_ = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        /// \brief Float sequence.
        Array<Float, 10> floats_ = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };

        /// \brief Integer constant sequence.
        Array<Int, 10> ones_ = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

        /// \brief Executed before each test case.
        void Before();
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& span_unit_test = MakeAutoUnitTest<SpanTestFixture>("span.core.syntropy")

    .TestCase("Default-constructed spans are empty.", [](auto& fixture)
    {
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(!empty, true);
        SYNTROPY_UNIT_EQUAL(empty.GetCount(), 0);
        SYNTROPY_UNIT_EQUAL(empty.GetData(), nullptr);
        SYNTROPY_UNIT_EQUAL(IsEmpty(empty), true);
        SYNTROPY_UNIT_EQUAL(Count(empty), 0);
    })

    .TestCase("Spans with zero elements are empty.", [](auto& fixture)
    {
        auto empty = SpanT<Int>{&fixture.ints_[0], 0};

        SYNTROPY_UNIT_EQUAL(!empty, true);
        SYNTROPY_UNIT_EQUAL(empty.GetData(), nullptr);
    })

    .TestCase("Spans constructed by a pair of equal iterators are empty.", [](auto& fixture)
    {
        auto empty = SpanT<Int>{ &fixture.ints_[0], &fixture.ints_[0] };

        SYNTROPY_UNIT_EQUAL(!empty, true);
        SYNTROPY_UNIT_EQUAL(empty.GetData(), nullptr);
    })

    .TestCase("Spans constructed from an iterator and a non-zero number of elements are non-empty.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(span0_10, true);
        SYNTROPY_UNIT_EQUAL(span0_10.GetCount(), 10);
        SYNTROPY_UNIT_EQUAL(span0_10.GetData(), &fixture.ints_[0]);
    })

    .TestCase("Spans constructed from a pair of non-equal iterators are non-empty.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], &fixture.ints_[0] + 10 };

        SYNTROPY_UNIT_EQUAL(!!span0_10, true);
        SYNTROPY_UNIT_EQUAL(span0_10.GetCount(), 10);
        SYNTROPY_UNIT_EQUAL(span0_10.GetData(), &fixture.ints_[0]);
    })

    .TestCase("Copy-constructed spans are identical.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span_copy = SpanT<Int>(span0_10);

        SYNTROPY_UNIT_EQUAL(span0_10, span_copy);
    })

    .TestCase("Copy-assigned spans are identical.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span_copy = span0_10;

        SYNTROPY_UNIT_EQUAL(span0_10, span_copy);
    })

    .TestCase("Constant spans can copy-constructed from non-constant spans.", [](auto& fixture)
    {
        auto rw_span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto rd_span0_10 = SpanT<const Int>(rw_span0_10);

        SYNTROPY_UNIT_EQUAL(rw_span0_10, rd_span0_10);
    })

    .TestCase("Spans provide read-only access to elements.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(span0_10[5], 5);
    })

    .TestCase("Read-write spans provide read-write access to elements.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        span0_10[5] = 42;

        SYNTROPY_UNIT_EQUAL(span0_10[5], 42);
    })

    .TestCase("Span front elements are readable.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        
        SYNTROPY_UNIT_EQUAL(Front(span0_10), span0_10[0]);
    })

    .TestCase("Span front elements are writable.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        Front(span0_10) = 42;

        SYNTROPY_UNIT_EQUAL(span0_10[0], 42);
    })

    .TestCase("Span back elements are readable.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Back(span0_10), span0_10[9]);
    })

    .TestCase("Span back elements are writable.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        Back(span0_10) = 42;

        SYNTROPY_UNIT_EQUAL(span0_10[9], 42);
    })

    .TestCase("Sub-spans constructed with all the elements in a span are both identical to each other.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Select(span0_10, 0, 10), span0_10);
    })

    .TestCase("Sub-spans with zero elements are empty.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Select(span0_10, 0, 0), SpanT<Int>{});
    })

    .TestCase("Sub-spans are identical to spans constructed explicitly with the same sequence.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span3_7 = SpanT<Int>{ &fixture.ints_[3], 5 };

        SYNTROPY_UNIT_EQUAL(Select(span0_10, 3, 5), span3_7);
    })

    .TestCase("Removing front elements from a span yields a sub-span which is equal to the remaining elements.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span1_9 = SpanT<Int>{ &fixture.ints_[1], 9 };
        auto span3_9 = SpanT<Int>{ &fixture.ints_[3], 7 };

        SYNTROPY_UNIT_EQUAL(PopFront(span0_10, 0), span0_10);
        SYNTROPY_UNIT_EQUAL(PopFront(span0_10), span1_9);
        SYNTROPY_UNIT_EQUAL(PopFront(span0_10, 3), span3_9);
    })

    .TestCase("Removing back elements from the span yields a sub-span which is equal to the remaining elements.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span0_8 = SpanT<Int>{ &fixture.ints_[0], 9 };
        auto span0_6 = SpanT<Int>{ &fixture.ints_[0], 7 };

        SYNTROPY_UNIT_EQUAL(PopBack(span0_10, 0), span0_10);
        SYNTROPY_UNIT_EQUAL(PopBack(span0_10), span0_8);
        SYNTROPY_UNIT_EQUAL(PopBack(span0_10, 3), span0_6);
    })

    .TestCase("Selecting the first elements of a span yields a sub-span which has the selected elements only.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span0_3 = SpanT<Int>{ &fixture.ints_[0], 4 };

        SYNTROPY_UNIT_EQUAL(Front(span0_10, 4), span0_3);
    })

    .TestCase("Selecting the last elements of a span yields a sub-span which has the selected elements only.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span6_9 = SpanT<Int>{ &fixture.ints_[6], 4 };

        SYNTROPY_UNIT_EQUAL(Back(span0_10, 4), span6_9);
    })

    .TestCase("Slicing a span from the front returns the front element and a sequence to the remaining ones.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span1_9 = SpanT<Int>{ &fixture.ints_[1], 9 };

        auto [front, back] = SliceFront(span0_10);

        SYNTROPY_UNIT_EQUAL(front, 0);
        SYNTROPY_UNIT_EQUAL(back, span1_9);
    })

    .TestCase("Slicing a span from the back returns the back element and a sequence to the remaining ones.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span0_9 = SpanT<Int>{ &fixture.ints_[0], 9 };

        auto [back, front] = SliceBack(span0_10);

        SYNTROPY_UNIT_EQUAL(front, span0_9);
        SYNTROPY_UNIT_EQUAL(back, 9);
    })

    .TestCase("Slicing a span from the front returns two sequences, one with the first elements and one with the remaining ones.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span0_3 = SpanT<Int>{ &fixture.ints_[0], 3 };
        auto span3_7 = SpanT<Int>{ &fixture.ints_[3], 7 };

        auto [front, back] = SliceFront(span0_10, 3);

        SYNTROPY_UNIT_EQUAL(front, span0_3);
        SYNTROPY_UNIT_EQUAL(back, span3_7);
    })

    .TestCase("Slicing a span from the back returns two sequences, one with the last elements and one with the remaining ones.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span7_3 = SpanT<Int>{ &fixture.ints_[7], 3 };
        auto span0_7 = SpanT<Int>{ &fixture.ints_[0], 7 };

        auto [back, front] = SliceBack(span0_10, 3);

        SYNTROPY_UNIT_EQUAL(back, span7_3);
        SYNTROPY_UNIT_EQUAL(front, span0_7);
    })

    .TestCase("The union of two non-overlapping, spans produce a span which contains both the element of the first and the second span.", [](auto& fixture)
    {
        auto span0_4 = SpanT<Int>{ &fixture.ints_[0], 4 };
        auto span6_9 = SpanT<Int>{ &fixture.ints_[6], 4 };
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Union(span0_4, span6_9), span0_10);
    })

    .TestCase("The union of two overlapping spans produce a span which contains both the element of the first and the second span and no duplicate.", [](auto& fixture)
    {
        auto span0_8 = SpanT<Int>{ &fixture.ints_[0], 8 };
        auto span2_9 = SpanT<Int>{ &fixture.ints_[2], 8 };
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Union(span0_8, span2_9), span0_10);
    })

    .TestCase("The union of a span with itself is identical to the span itself.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Union(span0_10, span0_10), span0_10);
    })

    .TestCase("The union of a span with am empty span is identical to the former.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(Union(span0_10, empty), span0_10);
    })

    .TestCase("The union of two spans is commutative.", [](auto& fixture)
    {
        auto span0_8 = SpanT<Int>{ &fixture.ints_[0], 8 };
        auto span2_9 = SpanT<Int>{ &fixture.ints_[2], 8 };

        SYNTROPY_UNIT_EQUAL(Union(span0_8, span2_9), Union(span2_9, span0_8));
    })

    .TestCase("The intersection of two non-overlapping ranges produce an empty span.", [](auto& fixture)
    {
        auto span0_5 = SpanT<Int>{ &fixture.ints_[0], 5 };
        auto span5_9 = SpanT<Int>{ &fixture.ints_[5], 5 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(Intersection(span0_5, span5_9), empty);
    })

    .TestCase("The intersection of two overlapping spans produce a span to the common sequence of the two.", [](auto& fixture)
    {
        auto span0_8 = SpanT<Int>{ &fixture.ints_[0], 8 };
        auto span2_9 = SpanT<Int>{ &fixture.ints_[2], 8 };
        auto span_2_7 = SpanT<Int>{ &fixture.ints_[2], 6 };

        SYNTROPY_UNIT_EQUAL(Intersection(span0_8, span2_9), span_2_7);
    })

    .TestCase("The intersection of a span with itself is identical to the span itself.", [](auto& fixture)
    {
        auto span0_5 = SpanT<Int>{ &fixture.ints_[0], 5 };

        SYNTROPY_UNIT_EQUAL(Intersection(span0_5, span0_5), span0_5);
    })

    .TestCase("The intersection of a span with the empty span is empty.", [](auto& fixture)
    {
        auto span0_5 = SpanT<Int>{ &fixture.ints_[0], 5 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(Intersection(span0_5, empty), empty);
    })

    .TestCase("The intersection of two spans is commutative.", [](auto& fixture)
    {
        auto span0_8 = SpanT<Int>{ &fixture.ints_[0], 8 };
        auto span2_9 = SpanT<Int>{ &fixture.ints_[2], 8 };
        
        SYNTROPY_UNIT_EQUAL(Intersection(span0_8, span2_9), Intersection(span2_9, span0_8));
    })

    .TestCase("The front difference of a span discards back elements until no element in the result is contained in the second operand and any element after that.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span5_6 = SpanT<Int>{ &fixture.ints_[5], 2 };
        auto span0_5 = SpanT<Int>{ &fixture.ints_[0], 5 };

        SYNTROPY_UNIT_EQUAL(DifferenceFront(span0_10, span5_6), span0_5);
    })

    .TestCase("The front difference of a span with itself is empty.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(DifferenceFront(span0_10, span0_10), empty);
    })

    .TestCase("The front difference of a span with the empty span leaves the span unchanged.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(DifferenceFront(span0_10, empty), span0_10);
    })

    .TestCase("The back difference of a span discards front elements until no element in the result is contained in the second operand and any element before that.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span1_3 = SpanT<Int>{ &fixture.ints_[1], 4 };
        auto span5_9 = SpanT<Int>{ &fixture.ints_[5], 5 };

        SYNTROPY_UNIT_EQUAL(DifferenceBack(span0_10, span1_3), span5_9);
    })

    .TestCase("The back difference of a span with itself is empty.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(DifferenceBack(span0_10, span0_10), empty);
    })

    .TestCase("The back difference of a span with the empty span leaves the span unchanged.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(DifferenceBack(span0_10, empty), span0_10);
    })

    .TestCase("Span contains sub-spans constructed from the same sequence.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span2_5 = SpanT<Int>{ &fixture.ints_[2], 4 };

        SYNTROPY_UNIT_EQUAL(Contains(span0_10, span2_5), true);
    })

    .TestCase("Spans contain themselves.", [](auto& fixture)
    {
        auto span0_3 = SpanT<Int>{ &fixture.ints_[0], 4 };

        SYNTROPY_UNIT_EQUAL(Contains(span0_3, span0_3), true);
    })

    .TestCase("Empty spans are contained in every other span.", [](auto& fixture)
    {
        auto span0_3 = SpanT<Int>{ &fixture.ints_[0], 4 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(Contains(span0_3, empty), true);
    })

    .TestCase("Empty spans contain no non-empty span.", [](auto& fixture)
    {
        auto span0_3 = SpanT<Int>{ &fixture.ints_[0], 4 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(Contains(empty, span0_3), false);
    })

    .TestCase("Empty spans contain themselves.", [](auto& fixture)
    {
        auto empty1 = SpanT<Int>{ &fixture.ints_[0], 0};
        auto empty2 = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(Contains(empty1, empty2), true);
    })

    .TestCase("Spans are identical to themselves.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(span0_10 == span0_10, true);
        SYNTROPY_UNIT_EQUAL(span0_10 != span0_10, false);
    })

    .TestCase("Empty spans are identical to other empty spans.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 0 };
        auto span0_10_a = SpanT<Int>{ &fixture.ints_a_[0], 0 };

        SYNTROPY_UNIT_EQUAL(span0_10 == span0_10, true);
        SYNTROPY_UNIT_EQUAL(span0_10 != span0_10, false);
    })

    .TestCase("Spans are identical to other spans constructed from the same sequence in memory.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span0_10_a = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(span0_10 == span0_10_a, true);
        SYNTROPY_UNIT_EQUAL(span0_10 != span0_10_a, false);
    })

    .TestCase("Spans differ from other spans constructed from different sequences in memory even if their elements compare equivalent.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span0_10_a = SpanT<Int>{ &fixture.ints_a_[0], 10 };

        SYNTROPY_UNIT_EQUAL(span0_10 == span0_10_a, false);
        SYNTROPY_UNIT_EQUAL(span0_10 != span0_10_a, true);
    })

    .TestCase("Spans are equivalent to themselves.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Equals(span0_10, span0_10), true);
    })

    .TestCase("Empty spans are equivalent to themselves.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 0 };
        auto span0_10_a = SpanT<Int>{ &fixture.ints_a_[0], 0 };

        SYNTROPY_UNIT_EQUAL(Equals(span0_10, span0_10_a), true);
    })

    .TestCase("Spans are equivalent to spans whose values compare equivalent.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span0_10_a = SpanT<Int>{ &fixture.ints_a_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Equals(span0_10, span0_10_a), true);
    })

    .TestCase("Spans are different from spans if exists an element in both which do not compare equal.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto ones0_9 = SpanT<Int>{ &fixture.ones_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Equals(span0_10, ones0_9), false);
    })

    .TestCase("Spans are equivalent to spans with a different type if the elements are implicitly convertible and compare equal.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto float0_9 = SpanT<Float>{ &fixture.floats_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Equals(span0_10, float0_9), true);
    })

    .TestCase("Span equality comparison is commutative.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto float0_9 = SpanT<Float>{ &fixture.floats_[0], 10 };

        SYNTROPY_UNIT_EQUAL(Equals(span0_10, float0_9), Equals(float0_9, span0_10));
    })
    
    .TestCase("Spans start-with themselves.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto empty = SpanT<Int>{};
         
        SYNTROPY_UNIT_EQUAL(StartsWith(span0_10, span0_10), true);
        SYNTROPY_UNIT_EQUAL(StartsWith(empty, empty), true);
    })

    .TestCase("Spans end-with themselves.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(EndsWith(span0_10, span0_10), true);
        SYNTROPY_UNIT_EQUAL(EndsWith(empty, empty), true);
    })

    .TestCase("Spans always start-with empty spans.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(StartsWith(span0_10, empty), true);
    })

    .TestCase("Spans always end-with empty spans.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(EndsWith(span0_10, empty), true);
    })

    .TestCase("Spans start-with spans whose value compare equivalent.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span0_4 = SpanT<Int>{ &fixture.ints_a_[0], 5 };

        SYNTROPY_UNIT_EQUAL(StartsWith(span0_10, span0_4), true);
    })

    .TestCase("Spans end-with spans whose values compare equivalent.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span5_9 = SpanT<Int>{ &fixture.ints_a_[5], 5 };

        SYNTROPY_UNIT_EQUAL(EndsWith(span0_10, span5_9), true);
    })

    .TestCase("Spans start-with spans with a different type when their elements are implicitly convertible and compare equal.", [](auto& fixture)
    {
        auto ints0_9 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto floats0_4 = SpanT<Float>{ &fixture.floats_[0], 5 };

        SYNTROPY_UNIT_EQUAL(StartsWith(ints0_9, floats0_4), true);
    })

    .TestCase("Spans end-with of spans with a different type when their elements are implicitly convertible and compare equal.", [](auto& fixture)
    {
        auto ints0_9 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto floats5_9 = SpanT<Float>{ &fixture.floats_[5], 5 };

        SYNTROPY_UNIT_EQUAL(EndsWith(ints0_9, floats5_9), true);
    })

    .TestCase("Spans don't-start-with spans whose member-wise equivalence is not met at least once.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span0_4 = SpanT<Int>{ &fixture.ones_[0], 5 };

        SYNTROPY_UNIT_EQUAL(StartsWith(span0_10, span0_4), false);
    })

    .TestCase("Spans don't-end-with spans whose member-wise equivalence is not met at least once.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span5_9 = SpanT<Int>{ &fixture.ones_[5], 5 };

        SYNTROPY_UNIT_EQUAL(EndsWith(span0_10, span5_9), false);
    })

    .TestCase("Empty spans don't-start-with non-empty spans.", [](auto& fixture)
    {
        auto empty = SpanT<Int>{};
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(StartsWith(empty, span0_10), false);
    })

    .TestCase("Empty spans don't-end-with non-empty spans.", [](auto& fixture)
    {
        auto empty = SpanT<Int>{};
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(EndsWith(empty, span0_10), false);
    })

    .TestCase("Spans have no prefix if the latter is longer than the former.", [](auto& fixture)
    {
        auto span0_4 = SpanT<Int>{ &fixture.ints_[0], 5 };
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(StartsWith(span0_4, span0_10), false);
    })

    .TestCase("Spans don't-start-with any other longer span.", [](auto& fixture)
    {
        auto span0_5 = SpanT<Int>{ &fixture.ints_a_[0], 5 };
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(StartsWith(span0_5, span0_10), false);
    })

    .TestCase("Spans don't-end-with any other longer span.", [](auto& fixture)
    {
        auto span0_5 = SpanT<Int>{ &fixture.ints_a_[0], 5 };
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };

        SYNTROPY_UNIT_EQUAL(EndsWith(span0_5, span0_10), false);
    })

    .TestCase("Searching a span for an element reduce the first until its front element compares equal to the provided element.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span3_9 = SpanT<Int>{ &fixture.ints_[3], 7 };

        SYNTROPY_UNIT_EQUAL(Find(span0_10, 3), span3_9);
    })

    .TestCase("Searching a span for an element reduce the first until its front element is implicitly convertible to the provided element.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span_3_7 = SpanT<Int>{ &fixture.ints_[3], 7 };

        SYNTROPY_UNIT_EQUAL(Find(span0_10, 3.0f), span_3_7);
    })

    .TestCase("Searching a span for a sub-span reduces the former until it starts-with the latter.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span6_1 = SpanT<Float>{ &fixture.floats_[6], 1 };
        auto span6_9 = SpanT<Int>{ &fixture.ints_[6], 4 };

        SYNTROPY_UNIT_EQUAL(Find(span0_10, span6_1), span6_9);
    })

    .TestCase("Searching a span for a sub-span reduces the former until it starts-with the latter, implicitly converting element types.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto span6_1 = SpanT<Float>{ &fixture.floats_[6], 1 };
        auto span6_9 = SpanT<Int>{ &fixture.ints_[6], 4 };

        SYNTROPY_UNIT_EQUAL(Find(span0_10, span6_1), span6_9);
    })

    .TestCase("Searching a span for a sub-span which is not contained in the original sequence returns an empty span.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto ones = SpanT<Int>{ &fixture.ones_[0], 4 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(Find(span0_10, ones), empty);
    })

    .TestCase("Searching for an empty span in another span returns the original span.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(Find(span0_10, empty), span0_10);
    })

    .TestCase("Searching an empty span for a non-empty span returns the empty span.", [](auto& fixture)
    {
        auto span0_10 = SpanT<Int>{ &fixture.ints_[0], 10 };
        auto empty = SpanT<Int>{};

        SYNTROPY_UNIT_EQUAL(Find(empty, span0_10), empty);
    });

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // SpanTestFixture.

    inline void SpanTestFixture::Before()
    {
        for (auto index = 0; index < 10; ++index)
        {
            ints_[index] = index;
            ints_a_[index] = index;
            floats_[index] = ToFloat(index);
            ones_[index] = 1;
        }
    }

}