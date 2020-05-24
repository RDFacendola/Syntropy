
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

        /// \brief Active test fixture context.
        static thread_local inline ObserverPtr<TestFixture> test_fixture_context_{ nullptr };

        /// \brief Previous test fixture context to restore upon destruction.
        ObserverPtr<TestFixture> previous_fixture_context_{ nullptr };

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
        auto test_fixture_t = TestFixture{ name_ };

        auto test_fixture_listener = syntropy::Listener{};

        test_fixture_listener += test_fixture_t.OnResult([this](const auto& sender, const auto& event_args)
        {
            result_event_.Notify(*this, { event_args.result_, event_args.message_, event_args.location_ });
        });

        test_fixture_listener += test_fixture_t.OnMessage([this](const auto& sender, const auto& event_args)
        {
            message_event_.Notify(*this, { event_args.message_ });
        });

        return test_fixture_t.Run(test_fixture, test_case_);
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