/// \file rational_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/rational.h"
#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::unit_test
{
    /************************************************************************/
    /* RATIONAL TEST FIXTURE                                                */
    /************************************************************************/

    /// \brief Rational test fixture.
    struct RationalTemplatesLanguageSyntropyFixture{};

    /************************************************************************/
    /* UNIT TESTS                                                           */
    /************************************************************************/

    inline const auto& rational_templates_language_syntropy_unit_test = MakeAutoUnitTest<RationalTestFixture>("rational.templates.language.syntropy")

    .TestCase("Adding two rational numbers together returns the unreduced sum of the twos.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Templates::RationalAdd<Templates::Rational<3, 6>, Templates::Rational<2, 5>>::numerator_, 27);
        SYNTROPY_UNIT_EQUAL(Templates::RationalAdd<Templates::Rational<3, 6>, Templates::Rational<2, 5>>::denominator_, 30);
    })

    .TestCase("Adding zero to a rational number results in the former.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(Templates::RationalAdd<Templates::Rational<3, 6>, Templates::Rational<0, 1>>::numerator_, 3);
        SYNTROPY_UNIT_EQUAL(Templates::RationalAdd<Templates::Rational<3, 6>, Templates::Rational<0, 1>>::denominator_, 6);
        SYNTROPY_UNIT_EQUAL(Templates::RationalAdd<Templates::Rational<0, 6>, Templates::Rational<3, 1>>::numerator_, 3);
        SYNTROPY_UNIT_EQUAL(Templates::RationalAdd<Templates::Rational<0, 6>, Templates::Rational<3, 1>>::denominator_, 1);
    });

}