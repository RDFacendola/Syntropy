#/// \file sequences_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/sequence.h"
#include "syntropy/language/templates/traits.h"
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

    .TestCase("MakeSequence<N> returns a contiguous integer sequence from 0 to N-1.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<MakeSequence<1>, Sequence<0>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<MakeSequence<5>, Sequence<0, 1, 2, 3, 4>>), true);
    })

    .TestCase("SequenceFor<T...> returns a contiguous integer sequence from 0 to sizeof...(T)", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<SequenceFor<Int, Float, Bool>, Sequence<0, 1, 2>>), true);
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<SequenceFor<Int>, Sequence<0>>), true);
    })

    .TestCase("SequenceFor returns an empty integer sequence if the parameter pack size is zero.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((Syntropy::Templates::IsSame<SequenceFor<>, Sequence<>>), true);
    })

    .TestCase("IsContiguousSequence returns true for contiguous sequences and false otherwise.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((IsContiguousSequence<Sequence<2, 3, 4>>), true);
        SYNTROPY_UNIT_EQUAL((IsContiguousSequence<Sequence<2, 5, 4>>), false);
    })

    .TestCase("IsContiguousSequence returns true for 1-sequences.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL((IsContiguousSequence<Sequence<2>>), true);
    });

}

// ===========================================================================