///
/// \file auto_unit_test.inl
///
/// \author Raffaele D. Facendola - 2020.

// ===========================================================================

#include "syntropy/diagnostics/unit_test/auto_test_case.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* AUTO UNIT TEST                                                       */
    /************************************************************************/

    template <typename TTestFixture>
    template <typename... TArguments>
    inline
    AutoUnitTest<TTestFixture>
    ::AutoUnitTest(Immutable<String> name,
                   Forwarding<TArguments>... arguments) noexcept
        : test_suite_(name, Forward<TArguments>(arguments)...)
    {

    }

    template <typename TTestFixture>
    template <typename TTestCase>
    inline Mutable<AutoUnitTest<TTestFixture>>
    AutoUnitTest<TTestFixture>
    ::TestCase(Immutable<String> name, Forwarding<TTestCase> test_case)
    noexcept
    {
        static
        auto inline_test_case = MakeAutoTestCase<TTestFixture>(name,
                                                               test_case);

        return *this;
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    template <typename TTestFixture, typename... TArguments>
    inline Mutable<AutoUnitTest<TTestFixture>>
    MakeAutoUnitTest(Immutable<String> name,
                     Forwarding<TArguments>... arguments) noexcept
    {
        static
        auto inline_unit_test
            = AutoUnitTest<TTestFixture>{ name,
                                          Forward<TArguments>(arguments)... };

        return inline_unit_test;
    }

}
