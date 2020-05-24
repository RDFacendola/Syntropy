
/// \file test_macros.h
/// \brief This header is part of the Syntropy unit test module. It contains unit test macros.
/// A test fixture is a stateful environment for multiple test cases.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/language/macro.h"
#include "syntropy/unit_test/test_fixture.h"

namespace syntropy
{
    /************************************************************************/
    /* UNIT TEST MACROS                                                     */
    /************************************************************************/

    /// \brief Unit test macro: report a success if "expression" is true, otherwise report a failure and return. Expected usage within a TestFixture.
    /// \usage SYNTROPY_UNIT_ASSERT(1 + 2 == 3);
    #define SYNTROPY_UNIT_ASSERT(expression) \
        SYNTROPY_MACRO_DECLARATION(expression)

    /// \brief Unit test macro: report a success if "expression" is true, otherwise report a failure. Expected usage within a TestFixture.
    /// Similar to SYNTROPY_UNIT_ASSERT except it doesn't return on failure.
    /// \usage SYNTROPY_UNIT_TEST(1 + 2 == 3);
    #define SYNTROPY_UNIT_TEST(expression) \
        SYNTROPY_MACRO_DECLARATION(expression)

    /// \brief Unit test macro: the test is executed if "expression" is true, otherwise the test is skipped. If used, it must precede any other test. Expected usage within a TestFixture.
    /// \usage SYNTROPY_UNIT_EXPECT(!IsServer());
    #define SYNTROPY_UNIT_EXPECT(expression) \
        SYNTROPY_MACRO_DECLARATION(expression)

    /// \brief Unit test macro: macro used to manually skip a test case. Expected usage within a TestFixture before any other test.
    /// \usage SYNTROPY_UNIT_SKIP("Work in progress");
    #define SYNTROPY_UNIT_SKIP(reason) \
        SYNTROPY_MACRO_DECLARATION(reason)

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

    #undef SYNTROPY_UNIT_ASSERT
    #define SYNTROPY_UNIT_ASSERT(expression) \
        if (bool result = (expression); !result) \
        { \
            syntropy::UnitTest::ReportTestCaseResult( syntropy::TestResult::kFailure, "ASSERT (" #expression ")", SYNTROPY_HERE ); \
            return; \
        } \
        else \
        { \
            syntropy::UnitTest::ReportTestCaseResult( syntropy::TestResult::kSuccess, "ASSERT (" #expression ")", SYNTROPY_HERE ); \
        }

    #undef SYNTROPY_UNIT_TEST
    #define SYNTROPY_UNIT_TEST(expression) \
        if (bool result = (expression); !result) \
        { \
            syntropy::UnitTest::ReportTestCaseResult( syntropy::TestResult::kFailure, "TEST (" #expression ")", SYNTROPY_HERE ); \
        } \
        else \
        { \
            syntropy::UnitTest::ReportTestCaseResult( syntropy::TestResult::kSuccess, "TEST (" #expression ")", SYNTROPY_HERE ); \
        }

    #undef SYNTROPY_UNIT_EXPECT
    #define SYNTROPY_UNIT_EXPECT(expression) \
        if(bool result = (expression); !result) \
        { \
            syntropy::UnitTest::ReportTestCaseResult( syntropy::TestResult::kSkipped, "EXPECT (" #expression ")", SYNTROPY_HERE ); \
            return; \
        } \
        else \
        { \
            syntropy::UnitTest::ReportTestCaseResult( syntropy::TestResult::kSuccess, "EXPECT (" #expression ")", SYNTROPY_HERE ); \
        }

    #undef SYNTROPY_UNIT_SKIP
    #define SYNTROPY_UNIT_SKIP(reason) \
        { \
            syntropy::UnitTest::ReportTestCaseResult( syntropy::TestResult::kSkipped, "SKIP (" #reason ")", SYNTROPY_HERE ); \
            return; \
        }

    #undef SYNTROPY_UNIT_MESSAGE
    #define SYNTROPY_UNIT_MESSAGE(...) \
        { \
            syntropy::UnitTest::ReportTestCaseMessage(__VA_ARGS__); \
        }

    #undef SYNTROPY_UNIT_TRACE
    #define SYNTROPY_UNIT_TRACE(expression) \
        { \
            (expression); \
            syntropy::UnitTest::ReportTestCaseMessage(#expression); \
        }

}