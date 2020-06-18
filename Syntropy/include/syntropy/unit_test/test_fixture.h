
/// \file test_fixture.h
/// \brief This header is part of the Syntropy unit test module. It contains classes used to define test fixtures.
/// A test fixture is a stateful environment for multiple test cases.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits>

#include "syntropy/diagnostics/stack_trace.h"
#include "syntropy/core/string.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/language/event.h"
#include "syntropy/language/type_traits.h"

#include "syntropy/unit_test/test_result.h"
#include "syntropy/unit_test/test_report.h"

namespace syntropy
{
    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    /// \brief Exposes test fixture-related functionalities used to notify results and messages from within a test fixture.
    /// \author Raffaele D. Facendola - May 2020
    namespace UnitTest
    {
        /// \brief Report a test case result in the current active test fixture.
        /// If no fixture is active, the behavior of this method is undefined.
        template <typename... TMessage>
        void ReportTestCaseResult(TestResult test_result, const StackTrace& test_location, TMessage&&... message);

        /// \brief Report a test case message in the current active test fixture.
        /// If no fixture is active, the behavior of this method is undefined.
        template <typename... TMessage>
        void ReportTestCaseMessage(TMessage&&... message);
    }

    /************************************************************************/
    /* ON TEST FIXTURE RESULT EVENT ARGS                                    */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test result is reported in the fixture.
    struct OnTestFixtureResultEventArgs
    {
        /// \brief Test result.
        TestResult result_;

        /// \brief Message.
        String message_;

        /// \brief Code location that issued the result.
        StackTrace location_;
    };

    /************************************************************************/
    /* ON TEST FIXTURE MESSAGE EVENT ARGS                                   */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test message is reported in the fixture.
    struct OnTestFixtureMessageEventArgs
    {
        /// \brief Reported message.
        String message_;
    };

    /************************************************************************/
    /* TEST FIXTURE                                                         */
    /************************************************************************/

    /// \brief Represents a stateful environment for multiple test cases.
    /// When a fixture is created it becomes active in the current scope. Fixtures can be nested but overlapping results in undefined behavior.
    /// \author Raffaele D. Facendola - January 2018
    class TestFixture
    {
        template <typename... TMessage>
        friend void UnitTest::ReportTestCaseResult(TestResult test_result, const StackTrace& test_location, TMessage&&... message);

        template <typename... TMessage>
        friend void UnitTest::ReportTestCaseMessage(TMessage&&... message);

    public:

        /// \brief Create a new test fixture.
        TestFixture();

        /// \brief Default copy-constructor.
        TestFixture(const TestFixture&) = default;

        /// \brief Default move-constructor.
        TestFixture(TestFixture&&) = default;

        /// \brief Default copy-assignment.
        TestFixture& operator=(const TestFixture&) = default;

        /// \brief Default move-assignment.
        TestFixture& operator=(TestFixture&&) = default;

        /// \brief Default virtual destructor.
        ~TestFixture();

        /// \brief Run a test case in a fixture.
        template <typename TTestFixture, typename TTestCase>
        TestReport Run(TTestFixture& test_fixture, const Label& test_name, TTestCase&& test_case) const;

        /// \brief Bind to the event notified whenever a result is reported.
        template <typename TDelegate>
        Listener OnResult(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a message is reported.
        template <typename TDelegate>
        Listener OnMessage(TDelegate&& delegate) const;

    private:

        /// \brief Detector for TTestFixture::After() member function.
        template <typename TTestFixture>
        using HasAfter = decltype(&TTestFixture::After);

        /// \brief Detector for TTestFixture::Before() member function.
        template <typename TTestFixture>
        using HasBefore = decltype(&TTestFixture::Before);

        /// \brief Report a result.
        void ReportResult(TestResult test_result, const String& test_message, const StackTrace& test_location) const;

        /// \brief Report a message.
        void ReportMessage(const String& test_message) const;

        /// \brief Active test fixture context.
        static thread_local inline ObserverPtr<TestFixture> context_{ nullptr };

        /// \brief Previous test fixture context to restore upon destruction.
        ObserverPtr<TestFixture> previous_context_{ nullptr };

        /// \brief Event notified whenever a test result is reported.
        Event<const TestFixture&, OnTestFixtureResultEventArgs> result_event_;

        /// \brief Event notified whenever a test message is reported.
        Event<const TestFixture&, OnTestFixtureMessageEventArgs> message_event_;

        /// \brief Test 
        Label name_;
    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // UnitTest.

    template <typename... TMessage>
    inline void UnitTest::ReportTestCaseResult(TestResult test_result, const StackTrace& test_location, TMessage&&... message)
    {
        auto builder = OStringStream{};

        (builder << ... << message);

        TestFixture::context_->ReportResult(test_result, builder.str(), test_location);
    }

    template <typename... TMessage>
    inline void UnitTest::ReportTestCaseMessage(TMessage&&... message)
    {
        auto builder = OStringStream{};

        (builder << ... << message);

        TestFixture::context_->ReportMessage(builder.str());
    }

    // TestFixture.

    inline TestFixture::TestFixture()
        : previous_context_(context_)
    {
        context_ = this;
    }

    inline TestFixture::~TestFixture()
    {
        context_ = previous_context_;
    }

    template <typename TTestFixture, typename TTestCase>
    inline TestReport TestFixture::Run(TTestFixture& test_fixture, const Label& test_name, TTestCase&& test_case) const
    {
        // Trick: set a listener to self to generate the report inside the function.

        auto test_report = MakeTestReport(test_name);

        auto report_listener = OnResult([&test_report](const auto& sender, const auto& event_args)
        {
            test_report += event_args.result_;
            test_report += event_args.location_;
        });

        // Run the test case.

        if constexpr (IsValidExpressionV<HasBefore, TTestFixture>)
        {
            test_fixture.Before();
        }

        (test_fixture.*test_case)();

        if constexpr (IsValidExpressionV<HasAfter, TTestFixture>)
        {
            test_fixture.After();
        }

        return test_report;
    }


    template <typename TDelegate>
    inline Listener TestFixture::OnResult(TDelegate&& delegate) const
    {
        return result_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener TestFixture::OnMessage(TDelegate&& delegate) const
    {
        return message_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    inline void TestFixture::ReportResult(TestResult test_result, const String& test_message, const StackTrace& test_location) const
    {
        result_event_.Notify(*this, { test_result, test_message, test_location });
    }

    inline void TestFixture::ReportMessage(const String& message) const
    {
        message_event_.Notify(*this, { message });
    }

}
