
/// \file test_suite.h
/// \brief This header is part of the syntropy unit test system. It contains classes and definitions for test suites.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <functional>
#include <type_traits>

#include "syntropy/unit_test/test_case.h"
#include "syntropy/unit_test/test_fixture.h"

#include "syntropy/containers/vector.h"
#include "syntropy/containers/context.h"

#include "syntropy/patterns/observable.h"

namespace syntropy
{

    /************************************************************************/
    /* TEST SUITE                                                           */
    /************************************************************************/

    /// \brief Base class for test suites.
    /// A suite is used to run tests declared by a test fixture.
    /// \author Raffaele D. Facendola - December 2017
    class TestSuite
    {
    public:

        /// \brief Arguments of the event called whenever a test suite starts.
        struct OnStartedEventArgs
        {

        };

        /// \brief Arguments of the event called whenever a running test suite finishes.
        struct OnFinishedEventArgs
        {
            TestResult result_;                                 ///< \brief Overall test result.
        };

        /// \brief Arguments of the event called whenever a new test case is ran.
        struct OnTestCaseStartedEventArgs
        {
            const TestCase& test_case_;                         ///< \brief Test case the event refers to.
        };

        /// \brief Arguments of the event called whenever a running test case finished.
        struct OnTestCaseFinishedEventArgs
        {
            const TestCase& test_case_;                         ///< \brief Test case the event refers to.

            TestResult result_;                                 ///< \brief Result of the test case.
        };

        /// \brief Arguments of the event called whenever a test case result is notified.
        struct OnTestCaseResultNotifiedEventArgs
        {
            const TestCase& test_case_;                         ///< \brief Test case the event refers to.

            TestResult result_;                                 ///< \brief Result.

            const String& message_;                        ///< \brief Result message.

            const diagnostics::StackTraceElement& location_;    ///< \brief Code that issued the result.
        };

        /// \brief Arguments of the event called whenever a test case notifies a message.
        struct OnTestCaseMessageNotifiedEventArgs
        {
            const TestCase& test_case_;                         ///< \brief Test case the event refers to.

            const String& message_;                        ///< \brief Notified message.
        };

        /// \brief Create a new named test suite bound to a test fixture.
        /// \param name Name of the test suite.
        /// \param arguments Arguments to pass to the fixture's constructor.
        template <typename TTestFixture, typename... TArguments>
        friend TestSuite MakeTestSuite(Context name, TArguments&&... arguments);

        /// \brief Get the test suite name.
        /// \return Returns the test suite name.
        const Context& GetName() const;

        /// \brief Get the test cases in this suite.
        /// \return Returns the test cases in this suite.
        const Vector<TestCase>& GetTestCases() const;

        /// \brief Run the test suite.
        /// \param context Context this suite is run on. Used to filter test suites by context.
        /// \return Returns the result of the test suite.
        TestResult Run(const Context& context) const;

        /// \brief Observable event called whenever this instance starts running tests.
        const Observable<const TestSuite&, const OnStartedEventArgs&>& OnStarted() const;

        /// \brief Observable event called whenever a this instance finished running tests.
        const Observable<const TestSuite&, const OnFinishedEventArgs&>& OnFinished() const;

        /// \brief Observable event called whenever a new test case is ran.
        const Observable<const TestSuite&, const OnTestCaseStartedEventArgs&>& OnTestCaseStarted() const;

        /// \brief Observable event called whenever a running test case finished.
        const Observable<const TestSuite&, const OnTestCaseFinishedEventArgs&>& OnTestCaseFinished() const;

        /// \brief Observable event called whenever a running test case notifies a result.
        const Observable<const TestSuite&, const OnTestCaseResultNotifiedEventArgs&>& OnTestCaseResultNotified() const;

        /// \brief Observable event called whenever a running test case notifies a message.
        const Observable<const TestSuite&, const OnTestCaseMessageNotifiedEventArgs&>& OnTestCaseMessageNotified() const;

    private:

        /// \brief Private constructor to avoid direct instantiation.
        /// \param name Name of the test suite.
        TestSuite(Context name);

        Context name_;                                                                                          ///< \brief Test suite name.

        std::function<std::unique_ptr<TestFixture>()> fixture_;                                                 ///< \brief Functor used to generate fixtures.

        Vector<TestCase> test_cases_;                                                                           ///< \brief Test cases to run.

        Event<const TestSuite&, const OnStartedEventArgs&> on_started_;                                        ///< \brief Event raised whenever this instance starts running tests.

        Event<const TestSuite&, const OnFinishedEventArgs&> on_finished_;                                       ///< \brief Event raised whenever this instance finished running tests.

        Event<const TestSuite&, const OnTestCaseStartedEventArgs&> on_test_case_started_;                       ///< \brief Event raised whenever a new test case started.

        Event<const TestSuite&, const OnTestCaseFinishedEventArgs&> on_test_case_finished_;                     ///< \brief Event raised whenever a running test case finished.

        Event<const TestSuite&, const OnTestCaseResultNotifiedEventArgs&> on_test_case_result_notified_;        ///< \brief Event raised whenever a running test case notifies a result.

        Event<const TestSuite&, const OnTestCaseMessageNotifiedEventArgs&> on_test_case_message_notified_;      ///< \brief Event raised whenever a running test case notifies a message.
    };

    /// \brief Create a new test suite by specifying a test fixture to bind.
    /// \param name Name of the test suite.
    /// \param arguments Arguments to pass to the fixture's constructor.
    /// \return Returns the new test suite.
    template <typename TTestFixture, typename... TArguments>
    TestSuite MakeTestSuite(Context name, TArguments&&... arguments)
    {
        static_assert(std::is_base_of_v<TestFixture, TTestFixture>, "TTestFixture must derive from TestFixture");

        static_assert(std::is_constructible_v<TTestFixture, TArguments...>, "TTestFixture must be constructible from TArguments...");

        TestSuite test_suite(std::move(name));

        test_suite.test_cases_ = TTestFixture::GetTestCases();

        test_suite.fixture_ = [arguments...]()
        {
            return std::make_unique<TTestFixture>(arguments...);
        };

        return test_suite;
    }
}

