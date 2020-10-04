/// \file tuple_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"

#include "syntropy/experimental/core/foundation/tuple.h"

#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::Experimental::UnitTest
{
    /************************************************************************/
    /* TUPLE TEST FIXTURE                                                   */
    /************************************************************************/

    /// \brief Alignment test fixture.
    struct TupleTestFixture
    {

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& alignment_unit_test = MakeAutoUnitTest<TupleTestFixture>("tuple.experimental.foundation.core.syntropy")

    .TestCase("", [](auto& fixture)
    {

    });

}