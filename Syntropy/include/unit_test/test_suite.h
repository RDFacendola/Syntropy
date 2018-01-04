
/// \file test_suite.h
/// \brief This header is part of the syntropy unit test system. It contains classes and definitions for test suites.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <functional>
#include <vector>
#include <type_traits>
#include <chrono>

#include "unit_test/test_case.h"
#include "unit_test/test_fixture.h"

#include "containers/context.h"

#include "patterns/observable.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST SUITE RESULT                                                    */
    /************************************************************************/

    /// \brief Test suite result.
    /// \author Raffaele D. Facendola - January 2018
    struct TestSuiteResult
    {
        TestResult result_{ TestResult::kSuccess };         ///< \brief Result of the test suite. The suite is considered successful only if each test case was either skipped or successful.
                                                            ///         Test cases failures and errors cause a suite to fail. Unhandled exception during test setup are considered errors.

        std::string message_;                               ///< \brief Test suite message.

        size_t count_{ 0 };                                 ///< \brief Number of test cases ran.

        size_t success_count_{ 0 };                         ///< \brief Number of successful test cases.

        size_t failure_count_{ 0 };                         ///< \brief Number of failed test cases.

        size_t error_count_{ 0 };                           ///< \brief Number of test cases aborted due to errors.

        size_t skip_count_{ 0 };                            ///< \brief Number of skipped test cases.

        /// \brief Add a new test case result.
        TestSuiteResult& operator +=(TestResult result);
    };

    /************************************************************************/
    /* TEST SUITE                                                           */
    /************************************************************************/

    /// \brief Base class for test suites.
    /// A suite is used to run tests declared by a test fixture.
    /// \author Raffaele D. Facendola - December 2017
    class TestSuite
    {
    public:

        /// \brief Arguments of the event called whenever a new test case is ran.
        struct OnTestCaseStartedEventArgs
        {
            const TestCase* test_case_;                         ///< \brief Test case the event refers to.
        };

        /// \brief Arguments of the event called whenever a running test case finished.
        struct OnTestCaseFinishedEventArgs
        {
            const TestCase* test_case_;                         ///< \brief Test case the event refers to.

            TestCaseResult result_;                             ///< \brief Result of the test case.

            std::chrono::milliseconds duration_;                ///< \brief Amount of time needed to execute the test case.
        };

        template <typename TTestFixture, typename... TArguments>
        friend TestSuite MakeTestSuite(Context name, TArguments&&... arguments);

        /// \brief Get the test suite name.
        /// \return Returns the test suite name.
        const Context& GetName() const;

        /// \brief Get the test cases in this suite.
        /// \return Returns the test cases in this suite.
        const std::vector<TestCase>& GetTestCases() const;

        /// \brief Run the test suite.
        /// \param context Context this suite is run on. Used to filter test suites by context.
        /// \return Returns the result of the test suite.
        TestSuiteResult Run(const Context& context) const;

        /// \brief Observable event called whenever a new test case is ran.
        const Observable<const TestSuite&, const OnTestCaseStartedEventArgs&>& OnTestCaseStarted() const;

        /// \brief Observable event called whenever a running test case finished.
        const Observable<const TestSuite&, const OnTestCaseFinishedEventArgs&>& OnTestCaseFinished() const;

    private:

        /// \brief Private constructor to avoid direct instantiation.
        /// \param name Name of the test suite.
        TestSuite(Context name);

        /// \brief Run a test case.
        /// \param fixture Test fixture the test case is run in.
        /// \param test_case Test case to run.
        /// \return Returns the result of the test.
        TestResult Run(TestFixture& fixture, const TestCase& test_case) const;

        Context name_;                                                                          ///< \brief Test suite name.

        std::function<std::unique_ptr<TestFixture>()> generate_fixture_;                        ///< \brief Functor used to generate new fixtures.

        std::unique_ptr<TestFixture> fixture_;                                                  ///< \brief Fixture this suite refers to.

        Event<const TestSuite&, const OnTestCaseStartedEventArgs&> on_test_case_started_;       ///< \brief Event raised whenever a new test case started.

        Event<const TestSuite&, const OnTestCaseFinishedEventArgs&> on_test_case_finished_;     ///< \brief Event raised whenever a running test case finished.
    };

    /// \brief Create a new test suite by specifying a test fixture to bind.
    /// \param name Name of the test suite.
    /// \param arguments Arguments to pass to the fixture's constructor.
    /// \return Returns the new test suite.
    template <typename TTestFixture, typename... TArguments>
    TestSuite MakeTestSuite(Context name, TArguments&&... arguments)
    {
        static_assert(std::is_base_of_v<TestFixture, TTestFixture>, "TTestFixture must derive from TestFixture");

        TestSuite test_suite(std::move(name));

        test_suite.fixture_ = std::make_unique<TTestFixture>(std::forward<TArguments>(arguments)...);

        test_suite.generate_fixture_ = []()
        {
            return std::make_unique<TTestFixture>(std::forward<TArguments>(arguments)...);
        };

        return test_suite;
    }
}

