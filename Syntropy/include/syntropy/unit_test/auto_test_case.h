
/// \file auto_test_case.h
/// \brief This header is part of the Syntropy unit test module. It contains classes used to define self-registering test cases.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/core/label.h"
#include "syntropy/core/smart_pointers.h"

#include "syntropy/unit_test/test_case.h"

namespace syntropy
{
    /************************************************************************/
    /* AUTO TEST CASE <TTEST FIXTURE>                                       */
    /************************************************************************/

    /// \brief Represents a self-registering test case for a test fixture.
    /// \usage const auto my_test_case = AutoTestCase<MyFixture>("foo", &MyFixture::Foo);
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TTestFixture>
    class AutoTestCase
    {
    public:

        /// \brief Type of a test case.
        using TTestCase = typename TestCase<TTestFixture>::TTestCase;

        /// \ brief Apply a function to each self-registering test case.
        template <typename TFunction>
        static void ForEach(TFunction&& function);

        /// \brief Create a new self-registering test case.
        AutoTestCase(const Label& name, TTestCase test_case);

        /// \brief No copy-constructor.
        AutoTestCase(const AutoTestCase&) = delete;

        /// \brief No move-constructor.
        AutoTestCase(AutoTestCase&&) = delete;

        /// \brief No copy-assignment.
        AutoTestCase& operator=(const AutoTestCase&) = delete;

        /// \brief No move-assignment.
        AutoTestCase& operator=(AutoTestCase&&) = delete;

        /// \brief Default destructor.
        ~AutoTestCase() = default;

        /// \brief Create a new test case.
        UniquePtr<TestCase<TTestFixture>> CreateTestCase() const;

    private:

        /// \brief Get the first element in a linked list to which every other self-registering test-case is linked to.
        static AutoTestCase& GetLinkedList();

        /// \brief Create an empty test case.
        AutoTestCase() = default;

        /// \brief Link this test-case to the others and return the next test-case after this one.
        ObserverPtr<const AutoTestCase> LinkBefore();

        /// \brief Test case name.
        Label name_;

        /// \brief Test case function.
        TTestCase test_case_{ nullptr };

        /// \brief Next auto test case in the fixture.
        ObserverPtr<const AutoTestCase> next_test_case_{ nullptr };

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create an self-registering test case by deducing the fixture type from arguments.
    /// \usage const auto my_test_case = MakeAutoTestCase(name, &MyFixture::Foo).
    template <typename TTestFixture>
    AutoTestCase<TTestFixture> MakeAutoTestCase(const Label& name, void(TTestFixture::* test_case)());

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AutoTestCase<TTestFixture>.

    template <typename TTestFixture>
    template <typename TFunction>
    inline void AutoTestCase<TTestFixture>::ForEach(TFunction&& function)
    {
        // Skip the very first test case as it is the sentinel to which every other test case is linked to.

        for (auto auto_test_case = GetLinkedList().next_test_case_; auto_test_case; auto_test_case = auto_test_case->next_test_case_)
        {
            function(*auto_test_case);
        }
    }

    template <typename TTestFixture>
    inline AutoTestCase<TTestFixture>::AutoTestCase(const Label& name, TTestCase test_case)
        : name_(name)
        , test_case_(test_case)
        , next_test_case_(LinkBefore())
    {

    }

    template <typename TTestFixture>
    inline UniquePtr<TestCase<TTestFixture>> AutoTestCase<TTestFixture>::CreateTestCase() const
    {
        return MakeUnique<TestCase<TTestFixture>>(name_, test_case_);
    }

    template <typename TTestFixture>
    inline AutoTestCase<TTestFixture>& AutoTestCase<TTestFixture>::GetLinkedList()
    {
        static auto linked_list = AutoTestCase{};

        return linked_list;
    }

    template <typename TTestFixture>
    inline ObserverPtr<const AutoTestCase<TTestFixture>> AutoTestCase<TTestFixture>::LinkBefore()
    {
        auto& linked_list = GetLinkedList();

        auto next = linked_list.next_test_case_;

        linked_list.next_test_case_ = this;

        return next;
    }

    // Non-member functions.

    template <typename TTestFixture>
    inline AutoTestCase<TTestFixture> MakeAutoTestCase(const Label& name, void(TTestFixture::* test_case)())
    {
        return { name, test_case };
    }

}
