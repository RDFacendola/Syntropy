#/// \file sequences_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/templates/sequence.h"
#include "syntropy/language/traits.h"
#include "syntropy/unit_test/unit_test.h"

// ===========================================================================

namespace Syntropy::Templates::UnitTest
{
    /************************************************************************/
    /* SEQUENCES TEST FIXTURE                                               */
    /************************************************************************/

    /// \brief Sequences test fixture.
    struct SequencesTestFixture {};

    /************************************************************************/
    /* UNIT TESTS                                                           */
    /************************************************************************/

    inline const auto& kSequencesUnitTest = MakeAutoUnitTest<MathTestFixture>("sequences.templates.language.syntropy")

    .TestCase("Templates::MakeIntegerSequence<N> returns a contiguous integer sequence from 0 to N-1.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::IsSame<Templates::MakeIntegerSequence<1>, Templates::IntegerSequence<0>>), true);
        SYNTROPY_UNIT_EQUAL((Templates::IsSame<Templates::MakeIntegerSequence<5>, Templates::IntegerSequence<0, 1, 2, 3, 4>>), true);
    })

    .TestCase("Templates::IntegerSequenceFor<T...> returns a contiguous integer sequence from 0 to sizeof...(T)", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::IsSame<Templates::IntegerSequenceFor<Int, Float, Bool>, Templates::IntegerSequence<0, 1, 2>>), true);
        SYNTROPY_UNIT_EQUAL((Templates::IsSame<Templates::IntegerSequenceFor<Int>, Templates::IntegerSequence<0>>), true);
    })

    .TestCase("Templates::IntegerSequenceFor returns an empty integer sequence if the parameter pack size is zero.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::IsSame<Templates::IntegerSequenceFor<>, Templates::IntegerSequence<>>), true);
    })

    .TestCase("Templates::IsContiguousSequence returns true for contiguous sequences and false otherwise.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::IsContiguousSequence<2, 3, 4>), true);
        SYNTROPY_UNIT_EQUAL((Templates::IsContiguousSequence<2, 5, 4>), false);
    })

    .TestCase("Templates::IsContiguousSequence returns true for 1-sequences.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Templates::IsContiguousSequence<2>), true);
    });

}

// ===========================================================================