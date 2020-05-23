
/// \file test_case.h
/// \brief This header is part of the syntropy unit test system. It contains classes used to define test cases.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits>

#include "syntropy/core/string.h"
#include "syntropy/core/label.h"
#include "syntropy/diagnostics/stack_trace.h"
#include "syntropy/unit_test/test_result.h"
#include "syntropy/unit_test/test_report.h"
#include "syntropy/unit_test/test_fixture.h"

namespace syntropy
{
    /************************************************************************/
    /* ON TEST CASE RESULT EVENT ARGS                                       */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a result is reported.
    struct OnTestCaseResultEventArgs : OnTestFixtureResultEventArgs
    {

    };

    /************************************************************************/
    /* ON TEST CASE MESSAGE EVENT ARGS                                      */
    /************************************************************************/

    /// \brief Arguments for the event notified whenever a message is reported.
    struct OnTestCaseMessageEventArgs : OnTestFixtureMessageEventArgs
    {

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

        /// \brief Default copy-constructor.
        TestCase(const TestCase&) = default;

        /// \brief Default move-constructor.
        TestCase(TestCase&&) = default;

        /// \brief Default copy-assignment.
        TestCase& operator=(const TestCase&) = default;

        /// \brief Default move-assignment.
        TestCase& operator=(TestCase&&) = default;

        /// \brief Default destructor.
        ~TestCase() = default;

        /// \brief Run the test case within a fixture and return a synthetic report.
        TestReport Run(TTestFixture& test_fixture);

        /// \brief Get the test case name.
        const Label& GetName() const;

        /// \brief Bind to the event notified whenever a result is reported. 
        template <typename TDelegate>
        Listener OnResult(TDelegate&& delegate);

        /// \brief Bind to the event notified whenever a message is reported. 
        template <typename TDelegate>
        Listener OnMessage(TDelegate&& delegate);

    private:

        /// \brief Test case name.
        Label name_;

        /// \brief Test case method.
        TTestCase test_case_;

        /// \brief Event notified whenever a result is reported.
        Event<TestCase&, OnTestCaseResultEventArgs> result_event_;

        /// \brief Event notified whenever a message is reported.
        Event<TestCase&, OnTestCaseMessageEventArgs> message_event_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a test case by deducing the fixture type from arguments.
    template <typename TTestFixture>
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

        auto notify_result_ = [this, &test_report](const OnTestFixtureResultEventArgs& event_args)
        {
            test_report += event_args.result_;
            test_report += event_args.location_;

            result_event_.Notify(*this, { event_args.result_, event_args.message_, event_args.location_ });
        };

        // Test fixture events.

        auto fixture_listener = syntropy::Listener{};

        fixture_listener += test_fixture.OnResult([notify_result_, this](const auto& sender, const auto& event_args)
        {
            notify_result_(event_args);
        });

        fixture_listener += test_fixture.OnMessage([this](const auto& sender, const auto& event_args)
        {
            message_event_.Notify(*this, { event_args.message_ });
        });

        // Test case environment.

        test_fixture.Before();

        (test_fixture.*test_case_)();

        test_fixture.After();

        return test_report;
    }

    template <typename TTestFixture>
    inline const Label& TestCase<TTestFixture>::GetName() const
    {
        return name_;
    }

    template <typename TTestFixture>
    template <typename TDelegate>
    inline Listener TestCase<TTestFixture>::OnResult(TDelegate&& delegate)
    {
        return result_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    template <typename TTestFixture>
    template <typename TDelegate>
    inline Listener TestCase<TTestFixture>::OnMessage(TDelegate&& delegate)
    {
        return message_event_.Subscribe(std::forward<TDelegate>(delegate));
    }

    // Non-member functions.

    template <typename TTestFixture>
    inline TestCase<TTestFixture> MakeTestCase(const Label& name, void(TTestFixture::* test_case)())
    {
        return { name, test_case };
    }

}