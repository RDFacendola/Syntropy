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

    /// \brief Test suite for Spans.
    struct SpanTestFixture
    {
        Int array[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    };

    inline const auto& span_unit_test = MakeAutoUnitTest<SpanTestFixture>("Span")

    .TestCase("DefaultSpanIsEmpty", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Span<Int>{}.GetCount(), 0);
        SYNTROPY_UNIT_EQUAL(!!Span<Int>{}, false);
    })

    .TestCase("SpanFront", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Span<Int>{ &fixture.array[1], 3 }).GetFront(), 1);

        auto span = Span<Int>{ &fixture.array[1], 3 };

        span.GetFront() = 42;

        SYNTROPY_UNIT_EQUAL(span.GetFront(), 42);
    })

    .TestCase("SpanBack", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Span<Int>{ &fixture.array[1], 3 }).GetBack(), 3);

        auto span = Span<Int>{ &fixture.array[1], 3 };

        span.GetBack() = 42;

        SYNTROPY_UNIT_EQUAL(span.GetBack(), 42);
    });

}