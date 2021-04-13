
/// \file empty_test_suite.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/diagnostics/unit_test/unit_test.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* EMPTY TEST FIXTURE                                                  */
    /************************************************************************/

    /// \brief Search test fixture.
    struct EmptyTestFixture
    {

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& empty_test_suite = Syntropy::UnitTest::MakeAutoUnitTest<EmptyTestFixture>(u8"empty")

    .TestCase(u8"Always true.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(42ll, 42ll);
    });
}

// ===========================================================================
