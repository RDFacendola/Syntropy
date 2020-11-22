
#/// \file compare_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/experimental/language/support/compare.h"

#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::UnitTest
{
    /************************************************************************/
    /* COMPARE TEST FIXTURE                                                 */
    /************************************************************************/

    /// \brief Compare test fixture.
    struct CompareTestFixture {};

    /************************************************************************/
    /* UNIT TESTS                                                           */
    /************************************************************************/

    inline const auto& kCompareUnitTest = MakeAutoUnitTest<CompareTestFixture>("compare.support.language.syntropy")

    .TestCase("", [](auto& fixture)
    {

    });

}