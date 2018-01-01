
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
    struct TestSuiteReport;

    /************************************************************************/
    /* TEST SUITE                                                           */
    /************************************************************************/

    /// \brief Base class for test suites.
    /// A suite is used to run tests declared by a test fixture.
    /// \author Raffaele D. Facendola - December 2017
    class TestSuite
    {
    public:

        /// \brief Arguments of the event called whenever a running test suite finishes.
        struct OnFinishedEventArgs
        {
            TestResult result_{ TestResult::kSuccess };         ///< \brief Result of the test suite. The suite is considered successful only if each test case was either skipped or successful.
                                                                ///         Test cases failures and errors cause a suite to fail. Unhandled exception during test setup are considered errors.

            std::string message_;                               ///< \brief Test suite message.

            size_t count_{ 0 };                                 ///< \brief Number of test cases ran.

            size_t success_count_{ 0 };                         ///< \brief Number of successful test cases.

            size_t failure_count_{ 0 };                         ///< \brief Number of failed test cases.

            size_t error_count_{ 0 };                           ///< \brief Number of test cases aborted due to errors.

            size_t skip_count_{ 0 };                            ///< \brief Number of skipped test cases.

            std::chrono::milliseconds duration_;                ///< \brief Amount of time needed to execute the test suite.

            /// \brief Add a new test case result.
            OnFinishedEventArgs& operator +=(TestResult result);
        };

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
        friend TestSuite MakeTestSuite(TArguments&&... arguments);

        /// \brief Get the test suite name.
        /// \return Returns the test suite name.
        const Context& GetName() const;

        /// \brief Get the test cases in this suite.
        /// \return Returns the test cases in this suite.
        const std::vector<TestCase>& GetTestCases() const;

        /// \brief Run the test suite.
        /// \param context Context this suite is run on. Used to filter test suites by context.
        void Run(const Context& context);

        /// \brief Observable event called whenever this suite is ran.
        Observable<TestSuite&>& OnStarted();

        /// \brief Observable event called whenever this suite finished running.
        Observable<TestSuite&, const OnFinishedEventArgs&>& OnFinished();

        /// \brief Observable event called whenever a new test case is ran.
        Observable<TestSuite&, const OnTestCaseStartedEventArgs&>& OnTestCaseStarted();

        /// \brief Observable event called whenever a running test case finished.
        Observable<TestSuite&, const OnTestCaseFinishedEventArgs&>& OnTestCaseFinished();

    private:

        /// \brief Private constructor to avoid direct instantiation.
        TestSuite() = default;

        /// \brief Run a test case.
        void RunTestCase(const TestCase& test_case);

        std::unique_ptr<TestFixture> fixture_;                                              ///< \brief Fixture this suite refers to.

        Event<TestSuite&> on_started_;                                                      ///< \brief Event raised whenever the suite is ran.

        Event<TestSuite&, const OnFinishedEventArgs&> on_finished_;                         ///< \brief Event raised whenever the suite finished running.

        Event<TestSuite&, const OnTestCaseStartedEventArgs&> on_test_case_started_;         ///< \brief Event raised whenever a new test case started.

        Event<TestSuite&, const OnTestCaseFinishedEventArgs&> on_test_case_finished_;       ///< \brief Event raised whenever a running test case finished.
    };

    /// \brief Create a new test suite by specifying a test fixture to bind.
    /// \param arguments Arguments to pass to the fixture's constructor.
    /// \return Returns the new test suite.
    template <typename TTestFixture, typename... TArguments>
    TestSuite MakeTestSuite(TArguments&&... arguments)
    {
        static_assert(std::is_base_of_v<TestFixture, TTestFixture>, "TTestFixture must derive from TestFixture");

        TestSuite test_suite;

        test_suite.fixture_ = std::make_unique<TTestFixture>(std::forward<TArguments>(arguments)...);

        return test_suite;
    }
}

