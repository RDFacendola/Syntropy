
/// \file auto_unit_test.h
///
/// \brief This header is part of the Syntropy diagnostics module.
/// \brief It contains definitions for self-registering unit tests.
///
/// \author Raffaele D. Facendola - 2020.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/strings/string.h"

#include "syntropy/diagnostics/unit_test/auto_test_suite.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* AUTO UNIT TEST                                                       */
    /************************************************************************/

    /// \brief A self-registering unit test bound to a test suite and many
    ///        test cases.
    ///
    /// Test-cases are provided as lambda with a single argument: the fixture.
    ///
    /// Usage:
    ///
    /// struct MyFixture{ ... };
    ///
    /// static auto
    ///     my_unit_test = MakeAutoUnitTest<MyFixture>("fixture.context")
    ///         .TestCase("testcase1", [](MyFixture& fixture){ ... }
    ///         ...
    ///         .TestCase(...);
    ///
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TTestFixture>
    class AutoUnitTest
    {
    public:

        /// \brief Create a new unit test for a fixture.
        template <typename... TArguments>
        AutoUnitTest(Immutable<String> name,
                     Forwarding<TArguments>... arguments) noexcept;

        /// \brief No copy-constructor.
        AutoUnitTest(Immutable<AutoUnitTest> rhs) = delete;

        /// \brief No move-constructor.
        AutoUnitTest(Movable<AutoUnitTest> rhs) = delete;

        /// \brief No copy-assignment.
        Mutable<AutoUnitTest>
        operator=(Immutable<AutoUnitTest> rhs) = delete;

        /// \brief No move-assignment.
        Mutable<AutoUnitTest>
        operator=(Movable<AutoUnitTest> rhs) = delete;

        /// \brief Default destructor.
        ~AutoUnitTest() noexcept = default;

        /// \brief Declare a new test case.
        template <typename TTestCase>
        Mutable<AutoUnitTest>
        TestCase(Immutable<String> name, Forwarding<TTestCase> test_case)
        noexcept;

    private:

        /// \brief Underlying test suite.
        AutoTestSuiteT<TTestFixture> test_suite_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new unique self-registering unit test bound to a
    ///        fixture.
    template <typename TTestFixture, typename... TArguments>
    Mutable<AutoUnitTest<TTestFixture>>
    MakeAutoUnitTest(Immutable<String> name,
                     Forwarding<TArguments>... arguments) noexcept;

}

// ===========================================================================

#include "details/auto_unit_test.inl"

// ===========================================================================
