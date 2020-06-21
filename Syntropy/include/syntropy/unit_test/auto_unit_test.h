/// \file auto_unit_test.h
/// \brief This header is part of the Syntropy unit test module. It contains definitions for self-registering unit tests.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/core/context.h"
#include "syntropy/core/label.h"

#include "syntropy/unit_test/test_suite.h"
#include "syntropy/unit_test/auto_test_suite.h"
#include "syntropy/unit_test/auto_test_case.h"

namespace syntropy
{
    /************************************************************************/
    /* AUTO UNIT TEST                                                       */
    /************************************************************************/

    /// \brief Represents a self-registering unit test bound to a test suite and many test cases.
    ///
    /// Usage:
    ///
    /// struct MyFixture{ ... };                                                                // This contain fixture data.
    ///
    /// static auto my_unit_test = MakeAutoUnitTest<MyFixture>("fixture.context")               // Test suite declaration.
    /// .TestCase("testcase1", [](MyFixture& fixture){ ... }                                    // Test case code goes here.
    /// ...
    /// .TestCase(...);                                                                         // Add as may test cases as you like.
    ///
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TTestFixture>
    class AutoUnitTest
    {
    public:

        /// \brief Create a new unit test for a fixture.
        template <typename... TArguments>
        AutoUnitTest(const Context& name, TArguments&&... arguments);

        /// \brief Declare a new test case.
        template <typename TTestCase>
        AutoUnitTest& TestCase(const Label& name, TTestCase&& test_case);

    private:

        /// \brief Underlying test suite.
        AutoTestSuiteT<TTestFixture> test_suite_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new unique self-registering unit test bound to a fixture.
    template <typename TTestFixture, typename... TArguments>
    AutoUnitTest<TTestFixture>& MakeAutoUnitTest(const Context& name, TArguments&&... arguments);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AutoUnitTest<TTestFixture>.

    template <typename TTestFixture>
    template <typename... TArguments>
    inline AutoUnitTest<TTestFixture>::AutoUnitTest(const Context& name, TArguments&&... arguments)
        : test_suite_(name, std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TTestFixture>
    template <typename TTestCase>
    inline AutoUnitTest<TTestFixture>& AutoUnitTest<TTestFixture>::TestCase(const Label& name, TTestCase&& test_case)
    {
        static auto inline_test_case = MakeAutoTestCase<TTestFixture>(name, test_case);

        return *this;
    }

    // Non-member functions.

    template <typename TTestFixture, typename... TArguments>
    inline AutoUnitTest<TTestFixture>& MakeAutoUnitTest(const Context& name, TArguments&&... arguments)
    {
        static auto auto_unit_test = AutoUnitTest<TTestFixture>(name, std::forward<TArguments>(arguments)...);

        return auto_unit_test;
    }

}
