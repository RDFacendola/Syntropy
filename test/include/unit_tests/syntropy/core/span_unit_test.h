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
        Int array_[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        Span<Int> empty_span_;

        Span<Int> nonempty_span_;

        Span<Int> copy_span_;

        void Before();
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& span_unit_test = MakeAutoUnitTest<SpanTestFixture>("Span")

    .TestCase("DefaultSpanIsEmpty", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.empty_span_.GetCount(), 0);
        SYNTROPY_UNIT_EQUAL(!!fixture.empty_span_, false);
    })

    .TestCase("SpanSize", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.nonempty_span_.GetCount(), 10);
        SYNTROPY_UNIT_EQUAL(!!fixture.nonempty_span_, true);
    })

    .TestCase("SpanFront", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.nonempty_span_.GetFront(), 0);

        fixture.nonempty_span_.GetFront() = 42;

        SYNTROPY_UNIT_EQUAL(fixture.nonempty_span_.GetFront(), 42);
    })

    .TestCase("SpanBack", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.nonempty_span_.GetBack(), 9);

        fixture.nonempty_span_.GetBack() = 42;

        SYNTROPY_UNIT_EQUAL(fixture.nonempty_span_.GetBack(), 42);
    })

    .TestCase("SpanRandomAccess", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.nonempty_span_[3], 3);

        fixture.nonempty_span_[3] = 42;

        SYNTROPY_UNIT_EQUAL(fixture.nonempty_span_[3], 42);
    })

    .TestCase("SpanIteration", [](auto& fixture)
    {
        auto index = 0;

        for (auto&& element : fixture.nonempty_span_)
        {
            SYNTROPY_UNIT_EQUAL(element, index++);
        }
    });

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // SpanTestFixture.

    inline void SpanTestFixture::Before()
    {
        empty_span_ = {};
        nonempty_span_ = { &array_[0], 10 };
        copy_span_ = nonempty_span_;
    }

}