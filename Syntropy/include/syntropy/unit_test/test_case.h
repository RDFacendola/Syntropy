
/// \file test_case.h
/// \brief This header is part of the syntropy unit test system. It contains classes used to define test cases.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits>

#include "syntropy/language/event.h"
#include "syntropy/language/macro.h"
#include "syntropy/core/string.h"
#include "syntropy/core/label.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/diagnostics/stack_trace.h"
#include "syntropy/unit_test/test_result.h"
#include "syntropy/unit_test/test_report.h"
#include "syntropy/unit_test/test_context.h"

namespace syntropy
{
    /************************************************************************/
    /* ON TEST CASE SUCCESS EVENT ARGS                                      */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a success is reported.
    struct OnTestCaseSuccessEventArgs : OnTestContextSuccessEventArgs
    {

    };

    /************************************************************************/
    /* ON TEST CASE FAILURE EVENT ARGS                                      */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a failure is reported.
    struct OnTestCaseFailureEventArgs : OnTestContextFailureEventArgs
    {

    };

    /************************************************************************/
    /* ON TEST CASE SKIPPED EVENT ARGS                                      */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a test case is skipped.
    struct OnTestCaseSkippedEventArgs : OnTestContextSkippedEventArgs
    {

    };

    /************************************************************************/
    /* ON TEST CASE MESSAGE EVENT ARGS                                      */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a message is reported.
    struct OnTestCaseMessageEventArgs : OnTestContextMessageEventArgs
    {

    };

    /************************************************************************/
    /* TEST CASE <TEST FIXTURE>                                             */
    /************************************************************************/

    /// \brief Represents an environment for a single test case.
    /// \author Raffaele D. Facendola - December 2017
    template <typename TTestFixture>
    class TestCase
    {
    public:

        /// \brief Create a named test case.
        TestCase(const Label& name);

        /// \brief Default copy-constructor.
        TestCase(const TestCase&) = default;

        /// \brief Default move-constructor.
        TestCase(TestCase&&) = default;

        /// \brief Default copy-assignment.
        TestCase& operator=(const TestCase&) = default;

        /// \brief Default move-assignment.
        TestCase& operator=(TestCase&&) = default;

        /// \brief Default virtual destructor.
        virtual ~TestCase() = default;

        /// \brief Run the test case within a fixture and return a synthetic report.
        TestReport Run(TTestFixture& test_fixture) const;

        /// \brief Get the test case name.
        const Label& GetName() const;

        /// \brief Bind to the event notified whenever a success is reported.
        template <typename TDelegate>
        Listener OnSuccess(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a failure is reported.
        template <typename TDelegate>
        Listener OnFailure(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a test is skipped.
        template <typename TDelegate>
        Listener OnSkipped(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a message is reported.
        template <typename TDelegate>
        Listener OnMessage(TDelegate&& delegate) const;

    private:

        /// \brief Run the concrete test case.
        virtual void RunTestCase(TTestFixture& test_fixture) const = 0;

        /// \brief Test case name.
        Label name_;

        /// \brief Event notified whenever a success is reported.
        Event<const TestCase&, OnTestCaseSuccessEventArgs> success_event_;

        /// \brief Event notified whenever a failure is reported.
        Event<const TestCase&, OnTestCaseFailureEventArgs> failure_event_;

        /// \brief Event notified whenever a test case is skipped.
        Event<const TestCase&, OnTestCaseSkippedEventArgs> skipped_event_;

        /// \brief Event notified whenever a message is reported.
        Event<const TestCase&, OnTestCaseMessageEventArgs> message_event_;

    };

    /************************************************************************/
    /* TEST CASE T <TEST FIXTURE, TEST CASE>                                */
    /************************************************************************/

    /// \brief Wraps a concrete test case method.
    /// \author Raffaele D. Facendola - December 2017
    template <typename TTestFixture, typename TTestCase>
    class TestCaseT : public TestCase<TTestFixture>
    {
    public:

        /// \brief Create a named test case.
        template <typename UTestCase>
        TestCaseT(const Label& name, UTestCase&& test_case);

        /// \brief Default copy-constructor.
        TestCaseT(const TestCaseT&) = default;

        /// \brief Default move-constructor.
        TestCaseT(TestCaseT&&) = default;

        /// \brief Default copy-assignment.
        TestCaseT& operator=(const TestCaseT&) = default;

        /// \brief Default move-assignment.
        TestCaseT& operator=(TestCaseT&&) = default;

        /// \brief Default virtual destructor.
        virtual ~TestCaseT() = default;

    private:

        virtual void RunTestCase(TTestFixture& test_fixture) const override;

        /// \brief Test case method.
        TTestCase test_case_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new test case by deducing templates from arguments.
    template <typename TTestFixture, typename TTestCase>
    TestCaseT<TTestFixture, TTestCase> MakeTestCase(const Label& name, TTestCase&& test_case);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // TestCase<TTestFixture>.

    template <typename TTestFixture>
    inline TestCase<TTestFixture>::TestCase(const Label& name)
        : name_(name)
    {

    }

    template <typename TTestFixture>
    TestReport TestCase<TTestFixture>::Run(TTestFixture& test_fixture) const
    {
        auto test_context = TestContext{};

        auto test_report = MakeTestReport(name_);

        auto context_listener = syntropy::Listener{};

        context_listener += test_context.OnSuccess([&test_report, this](const auto& sender, const auto& event_args)
        {
            success_event_.Notify(*this, { event_args.location_, event_args.expression_, event_args.result_ });
        });

        context_listener += test_context.OnFailure([&test_report, this](const auto& sender, const auto& event_args)
        {
            failure_event_.Notify(*this, { event_args.location_, event_args.expression_, event_args.result_, event_args.expected_ });
        });

        context_listener += test_context.OnSkipped([&test_report, this](const auto& sender, const auto& event_args)
        {
            skipped_event_.Notify(*this, { event_args.location_, event_args.reason_ });
        });

        context_listener += test_context.OnMessage([&test_report, this](const auto& sender, const auto& event_args)
        {
            message_event_.Notify(*this, { event_args.message_ });
        });

        RunTestCase(test_fixture);

        return test_report;
    }

    template <typename TTestFixture>
    inline const Label& TestCase<TTestFixture>::GetName() const
    {
        return name_;
    }

    template <typename TTestFixture>
    template <typename TDelegate>
    inline Listener TestCase<TTestFixture>::OnSuccess(TDelegate&& delegate) const
    {
        return success_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TTestFixture>
    template <typename TDelegate>
    inline Listener TestCase<TTestFixture>::OnFailure(TDelegate&& delegate) const
    {
        return failure_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TTestFixture>
    template <typename TDelegate>
    inline Listener TestCase<TTestFixture>::OnSkipped(TDelegate&& delegate) const
    {
        return skipped_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TTestFixture>
    template <typename TDelegate>
    inline Listener TestCase<TTestFixture>::OnMessage(TDelegate&& delegate) const
    {
        return message_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    // TestCaseT<TTestFixture, TTestCase>.
    
    template <typename TTestFixture, typename TTestCase>
    template <typename UTestCase>
    inline TestCaseT<TTestFixture, TTestCase>::TestCaseT(const Label& name, UTestCase&& test_case)
        : TestCase<TTestFixture>(name)
        , test_case_(std::forward<UTestCase>(test_case))
    {

    }

    template <typename TTestFixture, typename TTestCase>
    inline void TestCaseT<TTestFixture, TTestCase>::RunTestCase(TTestFixture& test_fixture) const
    {
        test_case_(test_fixture);
    }

    // Non-member functions.

    template <typename TTestFixture, typename TTestCase>
    inline TestCaseT<TTestFixture, TTestCase> MakeTestCase(const Label& name, TTestCase&& test_case)
    {
        return TestCaseT<TTestFixture, TTestCase>(name, std::forward<TTestCase>(test_case));
    }

}