
/// \file test_runner.h
/// \brief This header is part of the syntropy unit test system. It contains classes used to run test suites.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <memory>
#include <vector>
#include <type_traits>

#include "unit_test/test_suite.h"

#include "patterns/observable.h"

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

        /// \brief Arguments of the event called whenever a new test suite is ran.
        struct OnTestSuiteStartedEventArgs
        {
            TestSuite* test_suite_;                             ///< \brief Test suite the event refers to.
        };

        /// \brief Arguments of the event called whenever a running test suite finished.
        struct OnTestSuiteFinishedEventArgs
        {
            TestSuite* test_suite_;                             ///< \brief Test suite the event refers to.

            TestSuiteResult result_;                            ///< \brief Result of the test suite.

            std::chrono::milliseconds duration_;                ///< \brief Amount of time needed to execute the test case.
        };

        /// \brief Get the singleton instance.
        static TestRunner& GetInstance();

        /// \brief Run registered test suites.
        /// \param context Context used to filter test suites. Provide an empty context to run all the test suites available.
        void Run(const Context& context = "");

        /// \brief Get the test suites to run.
        /// \return Returns the test suites to run.
        const std::vector<TestSuite>& GetTestSuites() const;

        /// \brief Add a new test suit in-place.
        /// \param arguments Arguments to pass to the test fixture constructor.
        template <typename TTestFixture, typename... TArguments>
        void EmplaceTestSuite(TArguments&&... arguments)
        {
            test_suites_.emplace_back(MakeTestSuite<TTestFixture>(std::forward<TArguments>(arguments)...));
        }

        /// \brief Observable event called whenever a new test case is ran.
        Observable<TestRunner&, const OnTestSuiteStartedEventArgs&>& OnTestSuiteStarted();

        /// \brief Observable event called whenever a running test case finished.
        Observable<TestRunner&, const OnTestSuiteFinishedEventArgs&>& OnTestSuiteFinished();

    private:

        /// \brief Private constructor to avoid instantiation.
        TestRunner() = default;

        std::vector<TestSuite> test_suites_;                                                    ///< \brief Test suites to run.

        Event<TestRunner&, const OnTestSuiteStartedEventArgs&> on_test_suite_started_;          ///< \brief Event raised whenever a new test suite starts.

        Event<TestRunner&, const OnTestSuiteFinishedEventArgs&> on_test_suite_finished_;        ///< \brief Event raised whenever a running test suite finished.
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
    class AutoTestSuite
    {
        static_assert(std::is_base_of_v<TestFixture, TTestFixture>, "TTestFixture must derive from TestFixture");

    public:

        /// \brief Register a test suite to the TestRunner singleton instance.
        /// \param arguments Arguments to pass to TTestSuite constructor.
        template <typename... TArguments>
        AutoTestSuite(TArguments&&... arguments)
        {
            TestRunner::GetInstance().EmplaceTestSuite<TTestFixture>(std::forward<TArguments>(arguments)...);
        }
    };

}
