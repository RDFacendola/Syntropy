
/// \file test_macros.h
/// \brief This header is part of the Syntropy unit test module. It contains unit test macros.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/language/macro.h"
#include "syntropy/unit_test/test_context.h"

namespace syntropy
{
    /************************************************************************/
    /* UNIT TEST MACROS                                                     */
    /************************************************************************/

    /// \brief Unit test macro: report a success if "expression" is equal to expected, otherwise report a failure and continue. Expected usage within a TestFixture.
    /// \usage SYNTROPY_UNIT_EQUAL(1 + 2, 3);
    #define SYNTROPY_UNIT_EQUAL(expression, expected) \
        SYNTROPY_MACRO_DECLARATION(expression, expected)

    /// \brief Unit test macro: the test is executed if "expression" is true, otherwise the test is skipped. If used, it must precede any other test. Expected usage within a TestFixture.
    /// \usage SYNTROPY_UNIT_EXPECT(!IsServer());
    #define SYNTROPY_UNIT_EXPECT(expression) \
        SYNTROPY_MACRO_DECLARATION(expression)

    /// \brief Unit test macro: notify a message for the current test case being ran. Expected usage within a TestFixture.
    /// \usage SYNTROPY_UNIT_MESSAGE("This is a message ", 2 + 3);
    #define SYNTROPY_UNIT_MESSAGE(...) \
        SYNTROPY_MACRO_DECLARATION(__VA_ARGS__)

    /// \brief Unit test macro: execute "expression" and trace it as a message. Expected usage within a TestFixture.
    #define SYNTROPY_UNIT_TRACE(expression) \
        SYNTROPY_MACRO_DECLARATION(expression)

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Unit-test macros.

    #undef SYNTROPY_UNIT_EQUAL
    #define SYNTROPY_UNIT_EQUAL(expression, expected) \
        if (auto&& result = (expression); result != expected) \
        { \
            syntropy::UnitTest::ReportFailure( SYNTROPY_HERE, #expression, result, expected ); \
        } \
        else \
        { \
            syntropy::UnitTest::ReportSuccess( SYNTROPY_HERE, #expression, result ); \
        }

    #undef SYNTROPY_UNIT_EXPECT
    #define SYNTROPY_UNIT_EXPECT(expression) \
        if(Bool result = (expression); !result) \
        { \
            syntropy::UnitTest::ReportSkipped( SYNTROPY_HERE, #expression); \
            return; \
        }

    #undef SYNTROPY_UNIT_MESSAGE
    #define SYNTROPY_UNIT_MESSAGE(...) \
        syntropy::UnitTest::ReportMessage(__VA_ARGS__)

    #undef SYNTROPY_UNIT_TRACE
    #define SYNTROPY_UNIT_TRACE(expression) \
        syntropy::UnitTest::ReportMessage(#expression); \
        (expression);

}