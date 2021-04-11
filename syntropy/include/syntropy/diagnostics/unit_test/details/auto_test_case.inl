
/// \file auto_test_case.inl
///
/// \author Raffaele D. Facendola - 2020

#pragma once

// ===========================================================================

namespace Syntropy::UnitTest
{
    /************************************************************************/
    /* AUTO TEST CASE                                                       */
    /************************************************************************/

    template <typename TTestFixture>
    template <typename TFunction>
    inline void
    AutoTestCase<TTestFixture>
    ::ForEach(Forwarding<TFunction> function) noexcept
    {
        for (auto auto_test_case = GetLinkedList();
             auto_test_case;
             auto_test_case = auto_test_case->next_test_case_)
        {
            function(ReadOnly(*auto_test_case));
        }
    }

    template <typename TTestFixture>
    inline
    AutoTestCase<TTestFixture>
    ::AutoTestCase() noexcept
        : next_test_case_(LinkBefore())
    {

    }

    template <typename TTestFixture>
    inline Mutable<Ptr<AutoTestCase<TTestFixture>>>
    AutoTestCase<TTestFixture>
    ::GetLinkedList() noexcept
    {
        static auto linked_list = Ptr<AutoTestCase<TTestFixture>>{ nullptr };

        return linked_list;
    }

    template <typename TTestFixture>
    inline Ptr<AutoTestCase<TTestFixture>>
    AutoTestCase<TTestFixture>
    ::LinkBefore() noexcept
    {
        auto& linked_list = GetLinkedList();

        auto next_test_case = linked_list;

        linked_list = this;

        return next_test_case;
    }

    //

    template <typename TTestFixture, typename TTestCase>
    template <typename UTestCase>
    inline
    AutoTestCaseT<TTestFixture, TTestCase>
    ::AutoTestCaseT(Immutable<String> name, Forwarding<UTestCase> test_case)
    noexcept
        : test_case_(MakeTestCase<TTestFixture>(name,
                                                Forward<TTestCase>(test_case)))
    {

    }

    template <typename TTestFixture, typename TTestCase>
    inline Immutable<TestCase<TTestFixture>>
    AutoTestCaseT<TTestFixture, TTestCase>
    ::GetTestCase() const noexcept
    {
        return test_case_;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <typename TTestFixture, typename TTestCase>
    inline AutoTestCaseT<TTestFixture, TTestCase>
    MakeAutoTestCase(Immutable<String> name, Forwarding<TTestCase> test_case)
    noexcept
    {
        return { name, Forward<TTestCase>(test_case) };
    }

}
