
/// \file auto_test_suite.h
///
/// \brief This header is part of the Syntropy diagnostics module.
/// \brief It contains classes used to define self-registering test suites.
///
/// \author Raffaele D. Facendola - 2020

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/strings/string.h"

#include "syntropy/diagnostics/unit_test/test_suite.h"

// ===========================================================================

namespace Syntropy::UnitTest
{
    /************************************************************************/
    /* AUTO TEST SUITE                                                      */
    /************************************************************************/

    /// \brief Interface for self-registering test suites for a test fixture.
    ///
    /// \author Raffaele D. Facendola - May 2020.
    class AutoTestSuite
    {
    public:

        /// \ brief Apply a function to each self-registering test suite.
        template <typename TFunction>
        static void
        ForEach(Forwarding<TFunction> function) noexcept;

        /// \brief Create a new self-registering test suite.
        AutoTestSuite() noexcept;

        /// \brief No copy-constructor.
        AutoTestSuite(Immutable<AutoTestSuite> rhs) = delete;

        /// \brief No move-constructor.
        AutoTestSuite(Movable<AutoTestSuite> rhs) = delete;

        /// \brief No copy-assignment.
        Mutable<AutoTestSuite>
        operator=(Immutable<AutoTestSuite> rhs) = delete;

        /// \brief No move-assignment.
        Mutable<AutoTestSuite>
        operator=(Movable<AutoTestSuite>) = delete;

        /// \brief Default destructor.
        virtual
        ~AutoTestSuite() noexcept = default;

        /// \brief Run the test suite.
        virtual Immutable<TestSuite>
        GetTestSuite() const noexcept = 0;

    private:

        /// \brief Get the first element in a linked list to which every other
        ///        self-registering test-suite is linked to.
        static Mutable<Ptr<AutoTestSuite>>
        GetLinkedList() noexcept;

        /// \brief Link this test-suite to the others and return the next
        ///        test-suite after this one.
        Ptr<AutoTestSuite>
        LinkBefore() noexcept;

        /// \brief Next auto test suite.
        Ptr<AutoTestSuite> next_test_suite_{ nullptr };

    };

    /// \brief Represents a self-registering test suite for a test fixture.
    /// \tparam TFixtureArguments Type of the arguments used to construct the test fixture.
    ///
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TTestFixture>
    class AutoTestSuiteT : public AutoTestSuite
    {
    public:

        /// \brief Create a new self-registering test suite.
        template <typename... TFixtureArguments>
        AutoTestSuiteT(Immutable<String> name,
                       Forwarding<TFixtureArguments>... fixture_arguments)
        noexcept;

        /// \brief No copy-constructor.
        AutoTestSuiteT(Immutable<AutoTestSuiteT> rhs) = delete;

        /// \brief No move-constructor.
        AutoTestSuiteT(Movable<AutoTestSuiteT> rhs) = delete;

        /// \brief No copy-assignment.
        Mutable<AutoTestSuiteT>
        operator=(Immutable<AutoTestSuiteT> rhs) = delete;

        /// \brief No move-assignment.
        Mutable<AutoTestSuiteT>
        operator=(Movable<AutoTestSuiteT> rhs) = delete;

        /// \brief Default virtual destructor.
        virtual
        ~AutoTestSuiteT() noexcept = default;

    private:

        virtual Immutable<TestSuite>
        GetTestSuite() const noexcept override;

        /// \brief Underlying test_suite.
        TestSuiteT<TTestFixture> test_suite_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a self-registering test suite by deducing the fixture
    ///        type from arguments.
    ///
    /// \usage const auto auto_test_suite = MakeAutoTestSuite(name,
    ///                                                       arg0, arg1, ...).
    template <typename TTestFixture, typename... TFixtureArguments>
    AutoTestSuiteT<TTestFixture>
    MakeAutoTestSuite(Immutable<String> name,
                      Forwarding<TFixtureArguments>... arguments) noexcept;

}

// ===========================================================================

#include "details/auto_test_suite.inl"

// ===========================================================================
