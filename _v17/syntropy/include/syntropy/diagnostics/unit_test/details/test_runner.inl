
/// \file test_runner.inl
///
/// \author Raffaele D. Facendola - 2020.

// ===========================================================================

#include "syntropy/language/support/swap.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* TEST RUNNER                                                          */
    /************************************************************************/

    inline Mutable<TestRunner>
    TestRunner
    ::operator=(Movable<TestRunner> rhs) noexcept
    {
        Support::Swap(suite_started_event_, rhs.suite_started_event_);
        Support::Swap(suite_finished_event_, rhs.suite_finished_event_);
        Support::Swap(case_started_event_, rhs.case_started_event_);
        Support::Swap(case_finished_event_, rhs.case_finished_event_);
        Support::Swap(case_success_event_, rhs.case_success_event_);
        Support::Swap(case_failure_event_, rhs.case_failure_event_);

        return *this;
    }

    inline void
    TestRunner
    ::Run() const noexcept
    {
        AutoTestSuite::ForEach([this] (const auto& auto_test_suite)
        {
             Run(auto_test_suite.GetTestSuite());
        });
    }

    template <typename TDelegate>
    inline Listener
    TestRunner
    ::OnSuiteStarted(Forwarding<TDelegate> delegate) const noexcept
    {
        return suite_started_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener
    TestRunner
    ::OnSuiteFinished(Forwarding<TDelegate> delegate) const noexcept
    {
        return suite_finished_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener
    TestRunner
    ::OnCaseStarted(Forwarding<TDelegate> delegate) const noexcept
    {
        return case_started_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener
    TestRunner
    ::OnCaseFinished(Forwarding<TDelegate> delegate) const noexcept
    {
        return case_finished_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener
    TestRunner
    ::OnCaseSuccess(Forwarding<TDelegate> delegate) const noexcept
    {
        return case_success_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    template <typename TDelegate>
    inline Listener
    TestRunner
    ::OnCaseFailure(Forwarding<TDelegate> delegate) const noexcept
    {
        return case_failure_event_.Subscribe(Forward<TDelegate>(delegate));
    }

}