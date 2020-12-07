/// \file bytes_unit_test.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/memory/alignment.h"

#include "syntropy/unit_test/unit_test.h"

namespace Syntropy::Memory::UnitTest
{
    /************************************************************************/
    /* ALIGNMENT TEST FIXTURE                                               */
    /************************************************************************/

    /// \brief Alignment test fixture.
    struct AlignmentTestFixture
    {
        const Alignment alignment8_ = Alignment{ 8 };

        Alignment alignment_lhs_ = Alignment{ 16 };

        void Before();
    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline const auto& alignment_unit_test = MakeAutoUnitTest<AlignmentTestFixture>("alignment.memory.syntropy")

    .TestCase("Alignment are convertible to and from integers.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(ToInt(fixture.alignment8_), 8);
        SYNTROPY_UNIT_EQUAL(ToAlignment(8), fixture.alignment8_);
    })

    .TestCase("Alignment are convertible to and from Bytes.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(ToDataSize(fixture.alignment8_), Bytes{ 8 });
        SYNTROPY_UNIT_EQUAL(ToAlignment(Bytes{ 8 }), fixture.alignment8_);
    })

    .TestCase("Shifting an alignment left produces an alignment which is equal to an integer value shifted left by the same amount.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.alignment8_ >> 1, Alignment{ 4 });
        SYNTROPY_UNIT_EQUAL(fixture.alignment_lhs_ >> 1, Alignment{ 8 });
    })

    .TestCase("Shifting an alignment right produces an alignment which is equal to an integer value shifted right by the same amount.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.alignment8_ << 1, Alignment{ 16 });
        SYNTROPY_UNIT_EQUAL(fixture.alignment_lhs_ << 1, Alignment{ 32 });
    })

    .TestCase("Shifting left an alignment by a negative amount produces a result equal to right-shifting the same alignment by the absolute shifting value.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.alignment8_ >> -1, Alignment{ 16 });
        SYNTROPY_UNIT_EQUAL(fixture.alignment_lhs_ >> -1, Alignment{ 32 });
    })

    .TestCase("Shifting right an alignment by a negative amount produces a result equal to left-shifting the same alignment by the absolute shifting value.", [](auto& fixture)
    {
        SYNTROPY_UNIT_EQUAL(fixture.alignment8_ << -1, Alignment{ 4 });
        SYNTROPY_UNIT_EQUAL(fixture.alignment_lhs_ << -1, Alignment{ 8 });
    })

    .TestCase("Alignment literals behaves as metric prefixes equivalent.", [](auto& fixture)
    {
        using namespace Literals;

        SYNTROPY_UNIT_EQUAL(32_Alignment,  Alignment{ 32 });
    });

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    inline void AlignmentTestFixture::Before()
    {
        alignment_lhs_ = Alignment{ 16 };
    }

}