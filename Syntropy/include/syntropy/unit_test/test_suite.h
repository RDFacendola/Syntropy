
/// \file test_suite.h
/// \brief This header is part of the Syntropy unit test module. It contains classes used to define test suites.
/// A test fixture is a stateful environment for multiple test cases.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/core/context.h"
#include "syntropy/core/label.h"
#include "syntropy/language/event.h"

#include "syntropy/unit_test/test_report.h"
#include "syntropy/unit_test/test_case.h"
#include "syntropy/unit_test/auto_test_case.h"
#include "syntropy/unit_test/test_fixture.h"

namespace syntropy
{
    /************************************************************************/
    /* ON TEST SUITE STARTED EVENT ARGS                                     */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test starts.
    struct OnTestSuiteStartedEventArgs
    {
        /// \brief Test case name.
        Label test_case_;
    };

    /************************************************************************/
    /* ON TEST SUITE FINISHED EVENT ARGS                                    */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test ends.
    struct OnTestSuiteFinishedEventArgs
    {
        /// \brief Test case name.
        Label test_case_;

        /// \brief Synthetic test case report.
        TestReport test_report_;
    };

    /************************************************************************/
    /* ON TEST SUITE RESULT EVENT ARGS                                      */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test result is reported.
    struct OnTestSuiteResultEventArgs : OnTestCaseResultEventArgs
    {
        /// \brief Test case name.
        Label test_case_;
    };

    /************************************************************************/
    /* ON TEST SUITE MESSAGE EVENT ARGS                                     */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test is reported.
    struct OnTestSuiteMessageEventArgs : OnTestCaseMessageEventArgs
    {
        /// \brief Test case name.
        Label test_case_;
    };

    /************************************************************************/
    /* TEST SUITE                                                           */
    /************************************************************************/

    /// \brief Base class for test suites.
    /// A test suite is used to execute all test cases in a given test fixture.
    /// Test suites are named using "context" in order to facilitate categorization.
    /// \author Raffaele D. Facendola - May 2020
    class TestSuite
    {
    public:

        /// \brief Create a new named test suite.
        TestSuite(const Context& name);

        /// \brief Default virtual destructor.
        virtual ~TestSuite() = default;

        /// \brief Get the test suite name.
        const Context& GetName() const;

        /// \brief Run all test cases in the suite.
        virtual TestReport Run() = 0;

        /// \brief Bind to the event notified whenever a test starts. 
        template <typename TDelegate>
        Listener OnStarted(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a test starts. 
        template <typename TDelegate>
        Listener OnFinished(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a test result is reported. 
        template <typename TDelegate>
        Listener OnResult(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a test message is reported. 
        template <typename TDelegate>
        Listener OnMessage(TDelegate&& delegate);

    protected:

        /// \brief Notify the start of a test.
        void NotifyStarted(const OnTestSuiteStartedEventArgs& event_args);

        /// \brief Notify the end of a test.
        void NotifyFinished(const OnTestSuiteFinishedEventArgs& event_args);

        /// \brief Notify a result within a test case.
        void NotifyResult(const OnTestSuiteResultEventArgs& event_args);

        /// \brief Notify a message within a test case.
        void NotifyMessage(const OnTestSuiteMessageEventArgs& event_args);

    private:

        /// \brief Test suite name.
        Context name_;

        /// \brief Event notified whenever a test starts.
        Event<TestSuite*, OnTestSuiteStartedEventArgs> started_event_;

        /// \brief Event notified whenever a test finishes.
        Event<TestSuite*, OnTestSuiteFinishedEventArgs> finished_event_;

        /// \brief Event notified whenever a test result is reported.
        Event<TestSuite*, OnTestSuiteResultEventArgs> result_event_;

        /// \brief Event notified whenever a test message is reported.
        Event<TestSuite*, OnTestSuiteMessageEventArgs> message_event_;
    };

    /************************************************************************/
    /* TEST SUIT T <TTEST FIXTURE>                                          */
    /************************************************************************/

    /// \brief Test suite for a test fixture.
    /// \author Raffaele D. Facendola - May 2020
    template <typename TTestFixture>
    class TestSuiteT : public TestSuite
    {
    public:

        /// \brief Create a new test suite.
        template <typename... TArguments>
        TestSuiteT(const Context& name, TArguments&&... arguments);

        /// \brief Default virtual destructor.
        virtual ~TestSuiteT() = default;

        virtual TestReport Run() override;

    private:

        /// \brief Run a test case.
        TestReport Run(TestCase<TTestFixture>& test_case);

        /// \brief Underlying test fixture.
        TTestFixture test_fixture_;

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // TestSuite.

    inline TestSuite::TestSuite(const Context& name)
        : name_(name)
    {

    }

    inline const Context& TestSuite::GetName() const
    {
        return name_;
    }

    template <typename TDelegate>
    inline Listener TestSuite::OnStarted(TDelegate&& delegate)
    {
        return started_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestSuite::OnFinished(TDelegate&& delegate)
    {
        return finished_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestSuite::OnResult(TDelegate&& delegate)
    {
        return result_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestSuite::OnMessage(TDelegate&& delegate)
    {
        return message_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    inline void TestSuite::NotifyStarted(const OnTestSuiteStartedEventArgs& event_args)
    {
        started_event_.Notify(this, event_args);
    }

    inline void TestSuite::NotifyFinished(const OnTestSuiteFinishedEventArgs& event_args)
    {
        finished_event_.Notify(this, event_args);
    }

    inline void TestSuite::NotifyResult(const OnTestSuiteResultEventArgs& event_args)
    {
        result_event_.Notify(this, event_args);
    }

    inline void TestSuite::NotifyMessage(const OnTestSuiteMessageEventArgs& event_args)
    {
        message_event_.Notify(this, event_args);
    }

    // TestSuiteT<TTestFixture>.

    template <typename TTestFixture>
    template <typename... TArguments>
    inline TestSuiteT<TTestFixture>::TestSuiteT(const Context& name, TArguments&&... arguments)
        : TestSuite(name)
        , test_fixture_(std::forward<TArguments>(arguments)...)
    {

    }

    template <typename TTestFixture>
    TestReport TestSuiteT<TTestFixture>::Run()
    {
        auto test_report = MakeTestReport(GetName());

        AutoTestCase<TTestFixture>::ForEach([this, &test_report](TestCase<TTestFixture>& test_case)
        {
            test_report += Run(test_case);
        });

        return test_report;
    }

    template <typename TTestFixture>
    TestReport TestSuiteT<TTestFixture>::Run(TestCase<TTestFixture>& test_case)
    {
        // Setup listeners for the current test case.

        auto test_case_listener = syntropy::Listener{};

        test_case_listener += test_case.OnResult([this](TestCase<TTestFixture>* sender, const OnTestCaseResultEventArgs& event_args)
        {
            NotifyResult({ event_args.result_, event_args.message_, event_args.location_, sender->GetName() });
        });

        test_case_listener += test_case.OnMessage([this](TestCase<TTestFixture>* sender, const OnTestCaseMessageEventArgs& event_args)
        {
            NotifyMessage({ event_args.message_, sender->GetName() });
        });

        // Run.

        NotifyStarted({ test_case.GetName() });

        auto test_report = test_case.Run(test_fixture_);

        NotifyFinished({ test_case.GetName(), test_report });

        // Report.

        return test_report;
    }

}
