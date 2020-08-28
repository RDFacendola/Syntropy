
/// \file test_case.h
/// \brief This header is part of the syntropy unit test system. It contains classes used to define test cases.
///
/// \author Raffaele D. Facendola - 2018

#pragma once

#include <type_traits>

#include "syntropy/core/event.h"
#include "syntropy/language/preprocessor.h"
#include "syntropy/core/string.h"
#include "syntropy/core/label.h"
#include "syntropy/core/string_stream.h"
#include "syntropy/core/smart_pointers.h"
#include "syntropy/diagnostics/stack_trace.h"
#include "syntropy/unit_test/test_context.h"

namespace Syntropy
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

        /// \brief Run the test case within a fixture.
        void Run(TTestFixture& test_fixture) const;

        /// \brief Get the test case name.
        const Label& GetName() const;

        /// \brief Bind to the event notified whenever a success is reported.
        template <typename TDelegate>
        Listener OnSuccess(TDelegate&& delegate) const;

        /// \brief Bind to the event notified whenever a failure is reported.
        template <typename TDelegate>
        Listener OnFailure(TDelegate&& delegate) const;

    private:

        /// \brief Run the concrete test case.
        virtual void RunTestCase(TTestFixture& test_fixture) const = 0;

        /// \brief Test case name.
        Label name_;

        /// \brief Event notified whenever a success is reported.
        Event<const TestCase&, OnTestCaseSuccessEventArgs> success_event_;

        /// \brief Event notified whenever a failure is reported.
        Event<const TestCase&, OnTestCaseFailureEventArgs> failure_event_;

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
    void TestCase<TTestFixture>::Run(TTestFixture& test_fixture) const
    {
        auto test_context = TestContext{};

        auto context_listener = Syntropy::Listener{};

        context_listener += test_context.OnSuccess([this](const auto& sender, const auto& event_args)
        {
            success_event_.Notify(*this, { event_args.location_, event_args.expression_ });
        });

        context_listener += test_context.OnFailure([this](const auto& sender, const auto& event_args)
        {
            failure_event_.Notify(*this, { event_args.location_, event_args.expression_, event_args.result_, event_args.expected_ });
        });

        RunTestCase(test_fixture);
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
        return success_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    template <typename TTestFixture>
    template <typename TDelegate>
    inline Listener TestCase<TTestFixture>::OnFailure(TDelegate&& delegate) const
    {
        return failure_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    // TestCaseT<TTestFixture, TTestCase>.
    
    template <typename TTestFixture, typename TTestCase>
    template <typename UTestCase>
    inline TestCaseT<TTestFixture, TTestCase>::TestCaseT(const Label& name, UTestCase&& test_case)
        : TestCase<TTestFixture>(name)
        , test_case_(Forward<UTestCase>(test_case))
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
        return TestCaseT<TTestFixture, TTestCase>(name, Forward<TTestCase>(test_case));
    }

}