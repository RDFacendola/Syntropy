/// \file span_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/unit_test/auto_test_suite.h"
#include "syntropy/unit_test/auto_test_case.h"
#include "syntropy/unit_test/test_macros.h"

#include "syntropy/experimental/core/span.h"

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

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& AutoSpanTestSuite = MakeAutoTestSuite<SpanTestFixture>("SpanTestSuite");

    inline const auto AutoSpanTestSuiteDefaultSpanIsEmpty = MakeAutoTestCase<SpanTestFixture>("DefaultSpanIsEmpty", [](SpanTestFixture& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Span<Int>{}.GetCount(), 0);
        SYNTROPY_UNIT_EQUAL(!!Span<Int>{}, false);
    });

    inline const auto AutoSpanTestSuiteSpanFront = MakeAutoTestCase<SpanTestFixture>("SpanFront", [](SpanTestFixture& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Span<Int>{ &fixture.array[1], 3 }).GetFront(), 1);

        auto span = Span<Int>{ &fixture.array[1], 3 };

        span.GetFront() = 42;

        SYNTROPY_UNIT_EQUAL(span.GetFront(), 42);
    });

    inline const auto AutoSpanTestSuiteSpanBack = MakeAutoTestCase<SpanTestFixture>("SpanBack", [](SpanTestFixture& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Span<Int>{ &fixture.array[1], 3 }).GetBack(), 3);

        auto span = Span<Int>{ &fixture.array[1], 3 };

        span.GetBack() = 42;

        SYNTROPY_UNIT_EQUAL(span.GetBack(), 42);
    });

}