
/// \file auto_test_suite.inl
///
/// \author Raffaele D. Facendola - 2020

#pragma once

// ===========================================================================

namespace Syntropy::UnitTest
{
    /************************************************************************/
    /* AUTO TEST SUITE                                                      */
    /************************************************************************/

    template <typename TFunction>
    inline void
    AutoTestSuite
    ::ForEach(Forwarding<TFunction> function) noexcept
    {
        for (auto auto_test_suite = GetLinkedList();
             auto_test_suite;
             auto_test_suite = auto_test_suite->next_test_suite_)
        {
            function(ToImmutable(*auto_test_suite));
        }
    }

    inline
    AutoTestSuite
    ::AutoTestSuite() noexcept
        : next_test_suite_(LinkBefore())
    {

    }

    inline Mutable<Ptr<AutoTestSuite>>
    AutoTestSuite
    ::GetLinkedList() noexcept
    {
        static auto linked_list = Ptr<AutoTestSuite>{ nullptr };

        return linked_list;
    }

    inline Ptr<AutoTestSuite>
    AutoTestSuite
    ::LinkBefore() noexcept
    {
        auto& linked_list = GetLinkedList();

        auto next_test_suite = linked_list;

        linked_list = this;

        return next_test_suite;
    }

    //

    template <typename TTestFixture>
    template <typename... TFixtureArguments>
    inline
    AutoTestSuiteT<TTestFixture>
    ::AutoTestSuiteT(Immutable<String> name,
                     Forwarding<TFixtureArguments>... fixture_arguments)
    noexcept
        : test_suite_(name, Forward<TFixtureArguments>(fixture_arguments)...)
    {

    }

    template <typename TTestFixture>
    inline Immutable<TestSuite>
    AutoTestSuiteT<TTestFixture>
    ::GetTestSuite() const noexcept
    {
        return test_suite_;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <typename TTestFixture, typename... TFixtureArguments>
    inline AutoTestSuiteT<TTestFixture>
    MakeAutoTestSuite(Immutable<String> name,
                      Forwarding<TFixtureArguments>... arguments) noexcept
    {
        return { name, Forward<TFixtureArguments>(arguments)... };
    }

}
