
/// \file test_suite.h
/// \brief This header is part of the Syntropy unit test module. It contains classes used to define test suites.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include "syntropy/core/context.h"
#include "syntropy/core/label.h"
#include "syntropy/language/event.h"

#include "syntropy/unit_test/test_report.h"
#include "syntropy/unit_test/test_case.h"
#include "syntropy/unit_test/auto_test_case.h"

namespace syntropy
{
    /************************************************************************/
    /* ON TEST SUITE CASE STARTED EVENT ARGS                                */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test case starts.
    struct OnTestSuiteCaseStartedEventArgs
    {
        /// \brief Test case name.
        Label test_case_;
    };

    /************************************************************************/
    /* ON TEST SUITE CASE FINISHED EVENT ARGS                               */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test case ends.
    struct OnTestSuiteCaseFinishedEventArgs
    {
        /// \brief Test case name.
        Label test_case_;

        /// \brief Synthetic test case report.
        TestReport test_report_;
    };

    /************************************************************************/
    /* ON TEST SUITE CASE SUCCESS EVENT ARGS                                */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test case success is reported.
    struct OnTestSuiteCaseSuccessEventArgs : OnTestCaseSuccessEventArgs
    {
        /// \brief Test case name.
        Label test_case_;
    };

    /************************************************************************/
    /* ON TEST SUITE CASE FAILURE EVENT ARGS                                */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test case success is reported.
    struct OnTestSuiteCaseFailureEventArgs : OnTestCaseFailureEventArgs
    {
        /// \brief Test case name.
        Label test_case_;
    };

    /************************************************************************/
    /* ON TEST SUITE CASE SKIPPED EVENT ARGS                                */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test case success is reported.
    struct OnTestSuiteCaseSkippedEventArgs : OnTestCaseSkippedEventArgs
    {
        /// \brief Test case name.
        Label test_case_;
    };

    /************************************************************************/
    /* ON TEST SUITE CASE MESSAGE EVENT ARGS                                */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test message is reported.
    struct OnTestSuiteCaseMessageEventArgs : OnTestCaseMessageEventArgs
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

        /// \brief Default copy-constructor.
        TestSuite(const TestSuite&) = default;

        /// \brief Default move-constructor.
        TestSuite(TestSuite&&) = default;

        /// \brief Default copy-assignment.
        TestSuite& operator=(const TestSuite&) = default;

        /// \brief Default move-assignment.
        TestSuite& operator=(TestSuite&&) = default;

        /// \brief Default virtual destructor.
        virtual ~TestSuite() = default;

        /// \brief Get the test suite name.
        const Context& GetName() const;

        /// \brief Run all test cases in the suite.
        virtual TestReport Run() const = 0;

        /// \brief Bind to the event notified whenever a test starts. 
        template <typename TDelegate>
        Listener OnCaseStarted(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a test starts. 
        template <typename TDelegate>
        Listener OnCaseFinished(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a test case success is reported.
        template <typename TDelegate>
        Listener OnCaseSuccess(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a test case failure is reported.
        template <typename TDelegate>
        Listener OnCaseFailure(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a test case is skipped.
        template <typename TDelegate>
        Listener OnCaseSkipped(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a test message is reported. 
        template <typename TDelegate>
        Listener OnCaseMessage(TDelegate&& delegate) const;

    protected:

        /// \brief Notify the start of a test case.
        void NotifyCaseStarted(const OnTestSuiteCaseStartedEventArgs& event_args) const;

        /// \brief Notify the end of a test case.
        void NotifyCaseFinished(const OnTestSuiteCaseFinishedEventArgs& event_args) const;

        /// \brief Notify a success within a test case.
        void NotifyCaseSuccess(const OnTestSuiteCaseSuccessEventArgs& event_args) const;

        /// \brief Notify a failure within a test case.
        void NotifyCaseFailure(const OnTestSuiteCaseFailureEventArgs& event_args) const;

        /// \brief Notify a test case was skipped.
        void NotifyCaseSkip(const OnTestSuiteCaseSkippedEventArgs& event_args) const;

        /// \brief Notify a message within a test case.
        void NotifyCaseMessage(const OnTestSuiteCaseMessageEventArgs& event_args) const;

    private:

        /// \brief Test suite name.
        Context name_;

        /// \brief Event notified whenever a test case starts.
        Event<const TestSuite&, OnTestSuiteCaseStartedEventArgs> case_started_event_;

        /// \brief Event notified whenever a test case finishes.
        Event<const TestSuite&, OnTestSuiteCaseFinishedEventArgs> case_finished_event_;

        /// \brief Event notified whenever a success is reported.
        Event<const TestSuite&, OnTestSuiteCaseSuccessEventArgs> case_success_event_;

        /// \brief Event notified whenever a failure is reported.
        Event<const TestSuite&, OnTestSuiteCaseFailureEventArgs> case_failure_event_;

        /// \brief Event notified whenever a test case is skipped.
        Event<const TestSuite&, OnTestSuiteCaseSkippedEventArgs> case_skipped_event_;

        /// \brief Event notified whenever a test message is reported.
        Event<const TestSuite&, OnTestSuiteCaseMessageEventArgs> case_message_event_;
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

        /// \brief Default copy-constructor.
        TestSuiteT(const TestSuiteT&) = default;

        /// \brief Default move-constructor.
        TestSuiteT(TestSuiteT&&) = default;

        /// \brief Default copy-assignment.
        TestSuiteT& operator=(const TestSuiteT&) = default;

        /// \brief Default move-assignment.
        TestSuiteT& operator=(TestSuiteT&&) = default;

        /// \brief Default virtual destructor.
        virtual ~TestSuiteT() = default;

        virtual TestReport Run() const override;

    private:

        /// \brief Detector for UTestFixture::After() member function.
        template <typename UTestFixture>
        using HasAfter = decltype(std::declval<UTestFixture>().After());

        /// \brief Detector for UTestFixture::Before() member function.
        template <typename UTestFixture>
        using HasBefore = decltype(std::declval<UTestFixture>().Before());

        /// \brief Run a test case.
        TestReport Run(const TestCase<TTestFixture>& test_case) const;

        /// \brief Underlying test fixture.
        /// The fixture is not considered part of the external interface: test cases are either const or have to preserve the immutable state of the fixture via After and Before methods (which are required to be non-const).
        mutable TTestFixture test_fixture_;

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
    inline Listener TestSuite::OnCaseStarted(TDelegate&& delegate) const
    {
        return case_started_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestSuite::OnCaseFinished(TDelegate&& delegate) const
    {
        return case_finished_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestSuite::OnCaseSuccess(TDelegate&& delegate) const
    {
        return case_success_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestSuite::OnCaseFailure(TDelegate&& delegate) const
    {
        return case_failure_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestSuite::OnCaseSkipped(TDelegate&& delegate) const
    {
        return case_skipped_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestSuite::OnCaseMessage(TDelegate&& delegate) const
    {
        return case_message_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    inline void TestSuite::NotifyCaseStarted(const OnTestSuiteCaseStartedEventArgs& event_args) const
    {
        case_started_event_.Notify(*this, event_args);
    }

    inline void TestSuite::NotifyCaseFinished(const OnTestSuiteCaseFinishedEventArgs& event_args) const
    {
        case_finished_event_.Notify(*this, event_args);
    }

    inline void TestSuite::NotifyCaseSuccess(const OnTestSuiteCaseSuccessEventArgs& event_args) const
    {
        case_success_event_.Notify(*this, event_args);
    }

    inline void TestSuite::NotifyCaseFailure(const OnTestSuiteCaseFailureEventArgs& event_args) const
    {
        case_failure_event_.Notify(*this, event_args);
    }

    inline void TestSuite::NotifyCaseSkip(const OnTestSuiteCaseSkippedEventArgs& event_args) const
    {
        case_skipped_event_.Notify(*this, event_args);
    }

    inline void TestSuite::NotifyCaseMessage(const OnTestSuiteCaseMessageEventArgs& event_args) const
    {
        case_message_event_.Notify(*this, event_args);
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
    TestReport TestSuiteT<TTestFixture>::Run() const
    {
        auto test_report = MakeTestReport(GetName());

        AutoTestCase<TTestFixture>::ForEach([this, &test_report](const auto& auto_test_case)
        {
            test_report += Run(auto_test_case.GetTestCase());
        });

        return test_report;
    }

    template <typename TTestFixture>
    TestReport TestSuiteT<TTestFixture>::Run(const TestCase<TTestFixture>& test_case) const
    {
        // Setup listeners for the current test case.

        auto test_case_listener = syntropy::Listener{};

        test_case_listener += test_case.OnSuccess([this](const auto& sender, const auto& event_args)
        {
            NotifyCaseSuccess({ event_args.location_, event_args.expression_, event_args.result_, sender.GetName() });
        });

        test_case_listener += test_case.OnFailure([this](const auto& sender, const auto& event_args)
        {
            NotifyCaseFailure({ event_args.location_, event_args.expression_, event_args.result_, event_args.expected_, sender.GetName() });
        });

        test_case_listener += test_case.OnSkipped([this](const auto& sender, const auto& event_args)
        {
            NotifyCaseSkip({ event_args.location_, event_args.reason_, sender.GetName() });
        });

        test_case_listener += test_case.OnMessage([this](const auto& sender, const auto& event_args)
        {
            NotifyCaseMessage({ event_args.location_, event_args.message_, sender.GetName() });
        });

        // Run the test case.

        NotifyCaseStarted({ test_case.GetName() });

        if constexpr (IsValidExpressionV<HasBefore, TTestFixture>)
        {
            test_fixture_.Before();         // Setup the test fixture. Optional.
        }

        auto test_report = test_case.Run(test_fixture_);

        if constexpr (IsValidExpressionV<HasAfter, TTestFixture>)
        {
            test_fixture_.After();          // Cleanup the test fixture. Optional.
        }

        NotifyCaseFinished({ test_case.GetName(), test_report });

        // Report.

        return test_report;
    }

}
