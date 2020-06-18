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
    /* SPAN TEST SUITE                                                      */
    /************************************************************************/

    /// \brief Test suite for ???
    struct SpanTestSuite
    {
        void EmptySpanHasZeroElements();
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto AutoSpanTestSuite = MakeAutoTestSuite<SpanTestSuite>("SpanTestSuite");

    inline const auto AutoEmptySpanHasZeroElements = MakeAutoTestCase("SpanTestSuite", &SpanTestSuite::EmptySpanHasZeroElements);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // FooTestSuite.
    
    inline void SpanTestSuite::EmptySpanHasZeroElements()
    {
        SYNTROPY_UNIT_EQUAL(1 + 1, 2);
    }
    
}