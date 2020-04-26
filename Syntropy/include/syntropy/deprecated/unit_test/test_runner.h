
/// \file test_runner.h
/// \brief This header is part of the syntropy unit test system. It contains classes used to run test suites.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <memory>
#include "syntropy/containers/vector.h"
#include <type_traits>

#include "syntropy/unit_test/test_suite.h"

#include "syntropy/patterns/observable.h"

namespace syntropy
{
    /************************************************************************/
    /* TEST RUNNER                                                          */
    /************************************************************************/

    /// \brief Singleton class used to run multiple test suites.
    /// \author Raffaele D. Facendola - December 2017
    class TestRunner
    {
    public:

        /// \brief Arguments of the event called whenever a test runner starts.
        struct OnStartedEventArgs
        {

        };

        /// \brief Arguments of the event called whenever a running test runner finishes.
        struct OnFinishedEventArgs
        {
            TestResult result_;                                 ///< \brief Overall test result.
        };

        /// \brief Arguments of the event called whenever a new test suite is ran.
        struct OnTestSuiteStartedEventArgs
        {
            const TestSuite& test_suite_;                       ///< \brief Test suite the event refers to.
        };

        /// \brief Arguments of the event called whenever a running test suite finished.
        struct OnTestSuiteFinishedEventArgs
        {
            const TestSuite& test_suite_;                       ///< \brief Test suite the event refers to.

            TestResult result_;                                 ///< \brief Result of the test suite.
        };

        /// \brief Arguments of the event called whenever a new test case is ran.
        struct OnTestCaseStartedEventArgs
        {
            const TestSuite& test_suite_;                       ///< \brief Test suite the event refers to.

            const TestCase& test_case_;                         ///< \brief Test case the event refers to.
        };

        /// \brief Arguments of the event called whenever a running test case finished.
        struct OnTestCaseFinishedEventArgs
        {
            const TestSuite& test_suite_;                       ///< \brief Test suite the event refers to.

            const TestCase& test_case_;                         ///< \brief Test case the event refers to.

            TestResult result_;                                 ///< \brief Result of the test case.
        };

        /// \brief Arguments of the event called whenever a test case result is notified.
        struct OnTestCaseResultNotifiedEventArgs
        {
            const TestSuite& test_suite_;                       ///< \brief Test suite the event refers to.

            const TestCase& test_case_;                         ///< \brief Test case the event refers to.

            TestResult result_;                                 ///< \brief Result.

            const String& message_;                        ///< \brief Result message.

            const diagnostics::StackTraceElement& location_;    ///< \brief Code that issued the result.
        };

        /// \brief Arguments of the event called whenever a test case notifies a message.
        struct OnTestCaseMessageNotifiedEventArgs
        {
            const TestSuite& test_suite_;                       ///< \brief Test suite the event refers to.

            const TestCase& test_case_;                         ///< \brief Test case the event refers to.

            const String& message_;                        ///< \brief Notified message.
        };

        /// \brief Get the singleton instance.
        static TestRunner& GetInstance();

        /// \brief Run registered test suites.
        /// \param context Context used to filter test suites. Provide an empty context to run all the test suites available.
        /// \return Returns the result of the tests. Won't return TestResult::kSkipped under any circumstance.
        TestResult Run(const Context& context = "") const;

        /// \brief Get the test suites to run.
        /// \return Returns the test suites to run.
        const Vector<TestSuite>& GetTestSuites() const;

        /// \brief Add a new test suite in-place.
        /// \param arguments Arguments to pass to the test fixture constructor.
        template <typename TTestFixture, typename... TArguments>
        void EmplaceTestSuite(TArguments&&... arguments)
        {
            test_suites_.emplace_back(MakeTestSuite<TTestFixture>(std::forward<TArguments>(arguments)...));
        }

        /// \brief Observable event called whenever this instance starts running tests.
        const Observable<const TestRunner&, const OnStartedEventArgs&>& OnStarted() const;

        /// \brief Observable event called whenever a this instance finished running tests.
        const Observable<const TestRunner&, const OnFinishedEventArgs&>& OnFinished() const;

        /// \brief Observable event called whenever a new test suite is ran.
        const Observable<const TestRunner&, const OnTestSuiteStartedEventArgs&>& OnTestSuiteStarted() const;

        /// \brief Observable event called whenever a running test suite finishes.
        const Observable<const TestRunner&, const OnTestSuiteFinishedEventArgs&>& OnTestSuiteFinished() const;

        /// \brief Observable event called whenever a new test case is ran.
        const Observable<const TestRunner&, const OnTestCaseStartedEventArgs&>& OnTestCaseStarted() const;

        /// \brief Observable event called whenever a running test case finishes.
        const Observable<const TestRunner&, const OnTestCaseFinishedEventArgs&>& OnTestCaseFinished() const;

        /// \brief Observable event called whenever a running test case notifies a result.
        const Observable<const TestRunner&, const OnTestCaseResultNotifiedEventArgs&>& OnTestCaseResultNotified() const;

        /// \brief Observable event called whenever a running test case notifies a message.
        const Observable<const TestRunner&, const OnTestCaseMessageNotifiedEventArgs&>& OnTestCaseMessageNotified() const;

    private:

        /// \brief Private constructor to avoid instantiation.
        TestRunner() = default;

        Vector<TestSuite> test_suites_;                                                                    ///< \brief Test suites to run.

        Event<const TestRunner&, const OnStartedEventArgs&> on_started_;                                        ///< \brief Event raised whenever this instance starts running tests.

        Event<const TestRunner&, const OnFinishedEventArgs&> on_finished_;                                      ///< \brief Event raised whenever this instance finished running tests.

        Event<const TestRunner&, const OnTestSuiteStartedEventArgs&> on_test_suite_started_;                    ///< \brief Event raised whenever a new test suite starts.

        Event<const TestRunner&, const OnTestSuiteFinishedEventArgs&> on_test_suite_finished_;                  ///< \brief Event raised whenever a running test suite finished.

        Event<const TestRunner&, const OnTestCaseStartedEventArgs&> on_test_case_started_;                      ///< \brief Event raised whenever a new test case starts.

        Event<const TestRunner&, const OnTestCaseFinishedEventArgs&> on_test_case_finished_;                    ///< \brief Event raised whenever a running test case finishes.

        Event<const TestRunner&, const OnTestCaseResultNotifiedEventArgs&> on_test_case_result_notified_;       ///< \brief Event raised whenever a running test case notifies a result.

        Event<const TestRunner&, const OnTestCaseMessageNotifiedEventArgs&> on_test_case_message_notified_;     ///< \brief Event raised whenever a running test case notifies a message.
    };

    /************************************************************************/
    /* AUTO TEST SUITE                                                      */
    /************************************************************************/

    /// \brief Automatically register a TestSuite to the TestRunner singleton.
    /// 
    /// Usage (my_test_suite.cpp):
    ///
    /// AutoTestSuite<MyTestFixture> auto_my_test_suite;                // The constructor will register MyTestSuite to the test runner.
    /// 
    /// \author Raffaele D. Facendola - December 2017
    template <typename TTestFixture>
    struct AutoTestSuite
    {
        /// \brief Register a test suite to the TestRunner singleton instance.
        /// \param name Name of the test suite.
        /// \param arguments Arguments to pass to TTestSuite constructor.
        template <typename... TArguments>
        AutoTestSuite(Context name, TArguments&&... arguments)
        {
            TestRunner::GetInstance().EmplaceTestSuite<TTestFixture>(std::move(name), std::forward<TArguments>(arguments)...);
        }
    };

}
