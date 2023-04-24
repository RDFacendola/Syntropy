
/// \file test_suite.inl
///
/// \author Raffaele D. Facendola - 2018.

// ===========================================================================

#include "syntropy/diagnostics/unit_test/auto_test_case.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* TEST SUITE                                                           */
    /************************************************************************/

    inline
    TestSuite
    ::TestSuite(Immutable<String> name) noexcept
        : name_(name)
    {

    }

    inline Mutable<TestSuite>
    TestSuite
    ::operator=(Movable<TestSuite> rhs) noexcept
    {
        name_ = Move(rhs.name_);
        case_started_event_ = Move(rhs.case_started_event_);
        case_finished_event_ = Move(rhs.case_finished_event_);
        case_success_event_ = Move(rhs.case_success_event_);
        case_failure_event_ = Move(rhs.case_failure_event_);

        return *this;
    }

    inline
    Immutable<String>
    TestSuite
    ::GetName() const noexcept
    {
        return name_;
    }

    template <typename TDelegate>
    inline Listener
    TestSuite
    ::OnCaseStarted(Forwarding<TDelegate> delegate) const noexcept
    {
        return case_started_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener
    TestSuite
    ::OnCaseFinished(Forwarding<TDelegate> delegate) const noexcept
    {
        return case_finished_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener
    TestSuite
    ::OnCaseSuccess(Forwarding<TDelegate> delegate) const noexcept
    {
        return case_success_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener
    TestSuite
    ::OnCaseFailure(Forwarding<TDelegate> delegate) const noexcept
    {
        return case_failure_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    inline void
    TestSuite
    ::NotifyCaseStarted(
        Immutable<OnTestSuiteCaseStartedEventArgs> event_args) const noexcept
    {
        case_started_event_.Notify(*this, event_args);
    }

    inline void
    TestSuite
    ::NotifyCaseFinished(
        Immutable<OnTestSuiteCaseFinishedEventArgs> event_args) const noexcept
    {
        case_finished_event_.Notify(*this, event_args);
    }

    inline void
    TestSuite
    ::NotifyCaseSuccess(
        Immutable<OnTestSuiteCaseSuccessEventArgs> event_args) const noexcept
    {
        case_success_event_.Notify(*this, event_args);
    }

    inline void
    TestSuite
    ::NotifyCaseFailure(
        Immutable<OnTestSuiteCaseFailureEventArgs> event_args) const noexcept
    {
        case_failure_event_.Notify(*this, event_args);
    }

    //

    template <typename TTestFixture>
    template <typename... TArguments>
    inline
    TestSuiteT<TTestFixture>
    ::TestSuiteT(Immutable<String> name, Forwarding<TArguments>... arguments)
    noexcept
        : TestSuite(name)
        , test_fixture_(Forward<TArguments>(arguments)...)
    {

    }

    template <typename TTestFixture>
    inline Mutable<TestSuiteT<TTestFixture>>
    TestSuiteT<TTestFixture>
    ::operator=(Movable<TestSuiteT> rhs) noexcept
    {
        test_fixture_ = Move(rhs.test_fixture_);

        return *this;
    }

    template <typename TTestFixture>
    void
    TestSuiteT<TTestFixture>
    ::Run() const noexcept
    {
        AutoTestCase<TTestFixture>::ForEach([this](const auto& auto_test_case)
        {
            Run(auto_test_case.GetTestCase());
        });
    }

    template <typename TTestFixture>
    void
    TestSuiteT<TTestFixture>
    ::Run(Immutable<TestCase<TTestFixture>> test_case) const noexcept
    {
        // Setup listeners for the current test case.

        auto test_case_listener = Syntropy::Listener{};

        auto on_success = [this](const auto& sender, const auto& event_args)
        {
            NotifyCaseSuccess({ event_args.location_,
                                event_args.expression_,
                                sender.GetName() });
        };

        auto on_failure = [this](const auto& sender, const auto& event_args)
        {
            NotifyCaseFailure({ event_args.location_,
                                event_args.expression_,
                                event_args.result_,
                                event_args.expected_,
                                sender.GetName() });
        };

        test_case_listener += test_case.OnSuccess(on_success);
        test_case_listener += test_case.OnFailure(on_failure);

        // Run the test case.

        NotifyCaseStarted({ test_case.GetName() });

        if constexpr (requires{ { test_fixture_.Before() }; })
        {
            test_fixture_.Before();
        }

        test_case.Run(test_fixture_);

        if constexpr (requires{ { test_fixture_.After() }; })
        {
            test_fixture_.After();
        }

        NotifyCaseFinished({ test_case.GetName() });
    }

}
