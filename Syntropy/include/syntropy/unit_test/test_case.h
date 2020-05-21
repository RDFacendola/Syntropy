
/// \file test_case.h
/// \brief This header is part of the syntropy unit test system. It contains classes used to define test cases.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits>

#include "syntropy/core/string.h"
#include "syntropy/core/label.h"
#include "syntropy/language/event.h"
#include "syntropy/diagnostics/stack_trace.h"
#include "syntropy/unit_test/test_result.h"
#include "syntropy/unit_test/test_report.h"
#include "syntropy/unit_test/test_fixture.h"

namespace syntropy
{
    /************************************************************************/
    /* ON TEST CASE STARTED EVENT ARGS                                      */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test case starts.
    struct OnTestCaseStartedEventArgs
    {

    };

    /************************************************************************/
    /* ON TEST CASE FINISHED EVENT ARGS                                     */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test case ends.
    struct OnTestCaseFinishedEventArgs
    {
        /// \brief Synthetic test case report.
        TestReport test_report_;
    };

    /************************************************************************/
    /* TEST CASE <TTEST FIXTURE>                                            */
    /************************************************************************/

    /// \brief Represents an environment for a single test case.
    /// \author Raffaele D. Facendola - December 2017
    template <typename TTestFixture>
    class TestCase
    {
        static_assert(std::is_base_of_v<TestFixture, TTestFixture>, "TTestFixture must derive from TestFixture");

    public:

        /// \brief Type of a test case. Member function of TTestFixture.
        using TTestCase = void(TTestFixture::*)();

        /// \brief Create a named test case.
        TestCase(const Label& name, TTestCase test_case);

        /// \brief Run the test case within a fixture and return a synthetic report.
        TestReport Run(TTestFixture& test_fixture);

        /// \brief Get the test case name.
        const Label& GetName() const;

        /// \brief Bind to the event notified whenever a test case starts. 
        template <typename TDelegate>
        Listener OnTestCaseStarted(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a test case starts. 
        template <typename TDelegate>
        Listener OnTestCaseFinished(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a test case starts. 
        template <typename TDelegate>
        Listener OnTestCaseResult(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a test case starts. 
        template <typename TDelegate>
        Listener OnTestCaseMessage(TDelegate&& delegate);

    private:

        /// \brief Test case name.
        Label name_;

        /// \brief Test case method.
        TTestCase test_case_;

        /// \brief Event notified whenever a test case starts.
        Event<TestCase*, OnTestCaseStartedEventArgs> test_case_started_event_;

        /// \brief Event notified whenever a test case finishes.
        Event<TestCase*, OnTestCaseFinishedEventArgs> test_case_finished_event_;

        /// \brief Event notified whenever a test case result is reported.
        Event<TestCase*, OnTestCaseResultEventArgs> test_case_result_event_;

        /// \brief Event notified whenever a test case message is reported.
        Event<TestCase*, OnTestCaseMessageEventArgs> test_case_message_event_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a test case by deducing the fixture type from arguments.
    template <typename TTestFixture, typename >
    TestCase<TTestFixture> MakeTestCase(const Label& name, void(TTestFixture::* test_case)());

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // TestCase<TTestFixture>.

    template <typename TTestFixture>
    inline TestCase<TTestFixture>::TestCase(const Label& name, TTestCase test_case)
        : name_(name)
        , test_case_(test_case)
    {

    }

    template <typename TTestFixture>
    TestReport TestCase<TTestFixture>::Run(TTestFixture& test_fixture)
    {
        auto test_report = MakeTestReport(name_);

        auto notify_result_ = [this, &test_report](const OnTestCaseResultEventArgs& event_args)
        {
            test_report += event_args.result_;
            test_report += event_args.location_;

            test_case_result_event_.Notify(this, event_args);
        };

        // Test fixture events.

        auto fixture_listener = syntropy::Listener{};

        fixture_listener += test_fixture.OnTestCaseResult([notify_result_, this](TestFixture* sender, const OnTestCaseResultEventArgs& event_args)
        {
            notify_result_(event_args);
        });

        fixture_listener += test_fixture.OnTestCaseMessage([this](TestFixture* sender, const OnTestCaseMessageEventArgs& event_args)
        {
            test_case_message_event_.Notify(this, event_args);
        });

        // Test case environment.

        test_case_started_event_.Notify(this, {});

        test_fixture.Before();

        try
        {
            // Actual test case.

            (test_fixture.*test_case_)();
        }
        catch (...)
        {
            // Exceptions are considered "regular" test results, with the exception that it not possible to know the actual throw location.

            notify_result_({ TestResult::kError, "Unhandled exception", test_report.end_trace_ });
        }

        test_fixture.After();

        test_case_finished_event_.Notify(this, { test_report });

        return test_report;
    }

    template <typename TTestFixture>
    inline const Label& TestCase<TTestFixture>::GetName() const
    {
        return name_;
    }

    template <typename TTestFixture>
    template <typename TDelegate>
    inline Listener TestCase<TTestFixture>::OnTestCaseStarted(TDelegate&& delegate)
    {
        return test_case_started_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TTestFixture>
    template <typename TDelegate>
    inline Listener TestCase<TTestFixture>::OnTestCaseFinished(TDelegate&& delegate)
    {
        return test_case_finished_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TTestFixture>
    template <typename TDelegate>
    inline Listener TestCase<TTestFixture>::OnTestCaseResult(TDelegate&& delegate)
    {
        return test_case_result_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TTestFixture>
    template <typename TDelegate>
    inline Listener TestCase<TTestFixture>::OnTestCaseMessage(TDelegate&& delegate)
    {
        return test_case_message_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    // Non-member functions.

    template <typename TTestFixture>
    TestCase<TTestFixture> MakeTestCase(const Label& name, void(TTestFixture::* test_case)())
    {
        return { name, test_case };
    }

}