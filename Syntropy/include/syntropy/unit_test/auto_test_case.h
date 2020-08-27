
/// \file auto_test_case.h
/// \brief This header is part of the Syntropy unit test module. It contains classes used to define self-registering test cases.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/fundamentals.h"
#include "syntropy/core/label.h"
#include "syntropy/core/smart_pointers.h"

#include "syntropy/unit_test/test_case.h"

namespace Syntropy
{
    /************************************************************************/
    /* AUTO TEST CASE <TTEST FIXTURE>                                       */
    /************************************************************************/

    /// \brief Represents a self-registering test case for a test fixture.
    /// \usage const auto my_test_case = AutoTestCase<MyFixture>("foo", callable(MyFixture));
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TTestFixture>
    class AutoTestCase
    {
    public:

        /// \ brief Apply a function to each self-registering test case.
        template <typename TFunction>
        static void ForEach(TFunction&& function);

        /// \brief Create a new self-registering test case.
        AutoTestCase();

        /// \brief No copy-constructor.
        AutoTestCase(const AutoTestCase&) = delete;

        /// \brief No move-constructor.
        AutoTestCase(AutoTestCase&&) = delete;

        /// \brief No copy-assignment.
        AutoTestCase& operator=(const AutoTestCase&) = delete;

        /// \brief No move-assignment.
        AutoTestCase& operator=(AutoTestCase&&) = delete;

        /// \brief Default virtual destructor.
        virtual ~AutoTestCase() = default;

        /// \brief Access the underlying test case.
        virtual const TestCase<TTestFixture>& GetTestCase() const = 0;

    private:

        /// \brief Get the first element in a linked list to which every other self-registering test-case is linked to.
        static Pointer<const AutoTestCase>& GetLinkedList();

        /// \brief Link this test-case to the others and return the next test-case after this one.
        Pointer<const AutoTestCase> LinkBefore();

        /// \brief Next auto test case in the fixture.
        Pointer<const AutoTestCase> next_test_case_{ nullptr };

    };

    /************************************************************************/
    /* AUTO TEST CASE T <TEST FIXTURE, TEST CASE>                           */
    /************************************************************************/

    /// \brief Represents a concrete self-registering test case for a test fixture.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TTestFixture, typename TTestCase>
    class AutoTestCaseT : public AutoTestCase<TTestFixture>
    {
    public:

        /// \brief Create a new self-registering test case.
        template <typename UTestCase>
        AutoTestCaseT(const Label& name, UTestCase&& test_case);

        /// \brief No copy-constructor.
        AutoTestCaseT(const AutoTestCaseT&) = delete;

        /// \brief No move-constructor.
        AutoTestCaseT(AutoTestCaseT&&) = delete;

        /// \brief No copy-assignment.
        AutoTestCaseT& operator=(const AutoTestCaseT&) = delete;

        /// \brief No move-assignment.
        AutoTestCaseT& operator=(AutoTestCaseT&&) = delete;

        /// \brief Default destructor.
        virtual ~AutoTestCaseT() = default;

    private:

        virtual const TestCase<TTestFixture>& GetTestCase() const override;

        /// \brief Underlying test case function.
        TestCaseT<TTestFixture, TTestCase> test_case_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a self-registering test case by deducing the fixture type from arguments.
    /// \usage const auto my_test_case = MakeAutoTestCase(name, LambdaTestCase).
    template <typename TTestFixture, typename TTestCase>
    AutoTestCaseT<TTestFixture, TTestCase> MakeAutoTestCase(const Label& name, TTestCase&& test_case);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // AutoTestCase<TTestFixture>.

    template <typename TTestFixture>
    template <typename TFunction>
    inline void AutoTestCase<TTestFixture>::ForEach(TFunction&& function)
    {
        for (auto auto_test_case = GetLinkedList(); auto_test_case; auto_test_case = auto_test_case->next_test_case_)
        {
            function(ReadOnly(*auto_test_case));
        }
    }

    template <typename TTestFixture>
    inline AutoTestCase<TTestFixture>::AutoTestCase()
        : next_test_case_(LinkBefore())
    {

    }

    template <typename TTestFixture>
    inline Pointer<const AutoTestCase<TTestFixture>>& AutoTestCase<TTestFixture>::GetLinkedList()
    {
        static auto linked_list = Pointer<const AutoTestCase<TTestFixture>>{ nullptr };

        return linked_list;
    }

    template <typename TTestFixture>
    inline Pointer<const AutoTestCase<TTestFixture>> AutoTestCase<TTestFixture>::LinkBefore()
    {
        auto& linked_list = GetLinkedList();

        auto next_test_case = linked_list;

        linked_list = this;

        return next_test_case;
    }

    // AutoTestCaseT<TTestFixture, TTestCase>.

    template <typename TTestFixture, typename TTestCase>
    template <typename UTestCase>
    inline AutoTestCaseT<TTestFixture, TTestCase>::AutoTestCaseT(const Label& name, UTestCase&& test_case)
        : test_case_(MakeTestCase<TTestFixture>(name, Forward<TTestCase>(test_case)))
    {

    }

    template <typename TTestFixture, typename TTestCase>
    inline const TestCase<TTestFixture>& AutoTestCaseT<TTestFixture, TTestCase>::GetTestCase() const
    {
        return test_case_;
    }

    // Non-member functions.

    template <typename TTestFixture, typename TTestCase>
    inline AutoTestCaseT<TTestFixture, TTestCase> MakeAutoTestCase(const Label& name, TTestCase&& test_case)
    {
        return { name, Forward<TTestCase>(test_case) };
    }

}
