
/// \file auto_test_case.h
///
/// \brief This header is part of the Syntropy diagnostics module.
/// \brief It contains classes used to define self-registering test cases.
///
/// \author Raffaele D. Facendola - 2020.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/strings/string.h"

#include "syntropy/diagnostics/unit_test/test_case.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* AUTO TEST CASE                                                       */
    /************************************************************************/

    /// \brief A self-registering test case for a test fixture.
    /// \usage my_test_case = AutoTestCase<MyFixture>("foo",
    ///                                               callable(MyFixture));
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TTestFixture>
    class AutoTestCase
    {
    public:

        /// \ brief Apply a function to each self-registering test case.
        template <typename TFunction>
        static void
        ForEach(Forwarding<TFunction> function) noexcept;

        /// \brief Create a new self-registering test case.
        AutoTestCase() noexcept;

        /// \brief No copy-constructor.
        AutoTestCase(Immutable<AutoTestCase> rhs) = delete;

        /// \brief No move-constructor.
        AutoTestCase(Movable<AutoTestCase>) = delete;

        /// \brief No copy-assignment.
        Mutable<AutoTestCase>
        operator=(Immutable<AutoTestCase> rhs) = delete;

        /// \brief No move-assignment.
        Mutable<AutoTestCase>
        operator=(Movable<AutoTestCase> rhs) = delete;

        /// \brief Default virtual destructor.
        virtual
        ~AutoTestCase() noexcept = default;

        /// \brief Access the underlying test case.
        virtual Immutable<TestCase<TTestFixture>>
        GetTestCase() const noexcept = 0;

    private:

        /// \brief Get the first element in a linked list to which every other
        ///        self-registering test-case is linked to.
        static Mutable<Ptr<AutoTestCase>>
        GetLinkedList() noexcept;

        /// \brief Link this test-case to the others and return the next
        ///        test-case after this one.
        Ptr<AutoTestCase>
        LinkBefore() noexcept;

        /// \brief Next auto test case in the fixture.
        Ptr<AutoTestCase> next_test_case_{ nullptr };

    };

    /// \brief Represents a concrete self-registering test case for a test
    ///        fixture.
    ///
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TTestFixture, typename TTestCase>
    class AutoTestCaseT : public AutoTestCase<TTestFixture>
    {
    public:

        /// \brief Create a new self-registering test case.
        template <typename UTestCase>
        AutoTestCaseT(Immutable<String> name, Forwarding<UTestCase> test_case)
        noexcept;

        /// \brief No copy-constructor.
        AutoTestCaseT(Immutable<AutoTestCaseT> rhs) = delete;

        /// \brief No move-constructor.
        AutoTestCaseT(Movable<AutoTestCaseT>) = delete;

        /// \brief No copy-assignment.
        Mutable<AutoTestCaseT>
        operator=(Immutable<AutoTestCaseT> rhs) = delete;

        /// \brief No move-assignment.
        Mutable<AutoTestCaseT>
        operator=(Movable<AutoTestCaseT> rhs) = delete;

        /// \brief Default destructor.
        virtual
        ~AutoTestCaseT() noexcept = default;

    private:

        virtual Immutable<TestCase<TTestFixture>>
        GetTestCase() const noexcept override;

        /// \brief Underlying test case function.
        TestCaseT<TTestFixture, TTestCase> test_case_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a self-registering test case by deducing the fixture type
    ///        from arguments.
    /// \usage const auto my_test_case = MakeAutoTestCase(name,
    ///                                                   [](){ ... });
    template <typename TTestFixture, typename TTestCase>
    AutoTestCaseT<TTestFixture, TTestCase>
    MakeAutoTestCase(Immutable<String> name, Forwarding<TTestCase> test_case)
    noexcept;

}

// ===========================================================================

#include "details/auto_test_case.inl"

// ===========================================================================
