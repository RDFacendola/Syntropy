
/// \file priority_unit_test.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/diagnostics/unit_test/unit_test.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* PRIORITY TEST FIXTURE                                                */
    /************************************************************************/

    /// \brief Priority test fixture.
    struct PriorityTestFixture
    {

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& priority_unit_test
        = Syntropy::MakeAutoUnitTest<PriorityTestFixture>(
            u8"priority.templates.language.syntropy")

    .TestCase(u8"Priorities are implicitly convertible to all lower "
              u8"priorities.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Priority<1>, Priority<0>>), true);

        SYNTROPY_UNIT_EQUAL((IsConvertible<Priority<2>, Priority<1>>), true);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Priority<2>, Priority<0>>), true);

        SYNTROPY_UNIT_EQUAL((IsConvertible<Priority<3>, Priority<2>>), true);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Priority<3>, Priority<1>>), true);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Priority<3>, Priority<0>>), true);
    })

    .TestCase(u8"Priorities are not convertible to any higher priority.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((IsConvertible<Priority<0>, Priority<1>>), false);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Priority<0>, Priority<2>>), false);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Priority<0>, Priority<3>>), false);

        SYNTROPY_UNIT_EQUAL((IsConvertible<Priority<1>, Priority<2>>), false);
        SYNTROPY_UNIT_EQUAL((IsConvertible<Priority<1>, Priority<3>>), false);

        SYNTROPY_UNIT_EQUAL((IsConvertible<Priority<2>, Priority<3>>), false);
    });

}

// ===========================================================================
