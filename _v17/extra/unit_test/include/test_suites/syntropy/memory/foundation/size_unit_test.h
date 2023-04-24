
/// \file size_unit_test.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/diagnostics/unit_test/unit_test.h"

#include "syntropy/core/comparisons/compare.h"
#include "syntropy/core/comparisons/ordering.h"

#include "syntropy/math/numbers.h"

// ===========================================================================

namespace SizeUnitTest
{
    namespace Sy = Syntropy;

    /************************************************************************/
    /* FIXTURE                                                              */
    /************************************************************************/

    /// \brief Fixture.
    struct Fixture {};

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& unit_test
        = Sy::MakeAutoUnitTest<Fixture>(u8"size.foundation.memory.syntropy")

    .TestCase(u8"",
    [](auto& fixture)
    {

    });

}

// ===========================================================================
