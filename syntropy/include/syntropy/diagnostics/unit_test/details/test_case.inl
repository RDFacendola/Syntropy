
/// \file test_case.inl
///
/// \author Raffaele D. Facendola - 2018

#pragma once

// ===========================================================================

namespace Syntropy::UnitTest
{
    /************************************************************************/
    /* TEST CASE                                                            */
    /************************************************************************/

    template <typename TTestFixture>
    inline
    TestCase<TTestFixture>
    ::TestCase(Immutable<String> name) noexcept
        : name_(name)
    {

    }

    template <typename TTestFixture>
    void
    TestCase<TTestFixture>
    ::Run(Mutable<TTestFixture> test_fixture) const noexcept
    {
        auto on_success = [this](const auto& sender, const auto& event_args)
        {
            success_event_.Notify(*this, { event_args.location_,
                                           event_args.expression_ });
        };

        auto on_failure = [this](const auto& sender, const auto& event_args)
        {
            failure_event_.Notify(*this, { event_args.location_,
                                           event_args.expression_,
                                           event_args.result_,
                                           event_args.expected_ });
        };

        auto test_context = TestContext{};

        auto context_listener = Syntropy::Listener{};

        context_listener += test_context.OnSuccess(on_success);
        context_listener += test_context.OnFailure(on_failure);

        RunTestCase(test_fixture);
    }

    template <typename TTestFixture>
    inline Immutable<String>
    TestCase<TTestFixture>
    ::GetName() const noexcept
    {
        return name_;
    }

    template <typename TTestFixture>
    template <typename TDelegate>
    inline Listener
    TestCase<TTestFixture>
    ::OnSuccess(Forwarding<TDelegate> delegate) const noexcept
    {
        return success_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    template <typename TTestFixture>
    template <typename TDelegate>
    inline Listener
    TestCase<TTestFixture>
    ::OnFailure(Forwarding<TDelegate> delegate) const noexcept
    {
        return failure_event_.Subscribe(Forward<TDelegate>(delegate));
    }

    template <typename TTestFixture, typename TTestCase>
    template <typename UTestCase>
    inline
    TestCaseT<TTestFixture, TTestCase>
    ::TestCaseT(Immutable<String> name, Forwarding<UTestCase> test_case)
    noexcept
        : TestCase<TTestFixture>(name)
        , test_case_(Forward<UTestCase>(test_case))
    {

    }

    template <typename TTestFixture, typename TTestCase>
    inline void
    TestCaseT<TTestFixture, TTestCase>
    ::RunTestCase(Mutable<TTestFixture> test_fixture) const noexcept
    {
        test_case_(test_fixture);
    }

    // Non-member functions.

    template <typename TTestFixture, typename TTestCase>
    inline TestCaseT<TTestFixture, TTestCase>
    MakeTestCase(Immutable<String> name, Forwarding<TTestCase> test_case)
    noexcept
    {
        return TestCaseT<TTestFixture, TTestCase>(name,
            Forward<TTestCase>(test_case));
    }

}
