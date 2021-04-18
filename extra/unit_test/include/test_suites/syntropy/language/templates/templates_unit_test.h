
/// \file invoke_unit_test.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/templates.h"
#include "syntropy/language/templates/concepts.h"

#include "syntropy/diagnostics/unit_test/unit_test.h"

#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* TEMPLATES TEST FIXTURE                                               */
    /************************************************************************/

    /// \brief Templates test fixture.
    struct TemplatesTestFixture
    {

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& templates_unit_test
        = Syntropy::UnitTest::MakeAutoUnitTest<TemplatesTestFixture>(
            u8"templates.templates.language.syntropy")

    .TestCase(u8"Alias types provide a member Type equal to the alias type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME(Alias<Int>::Type, Int);
    })

    .TestCase(u8"Constant types provide a member kValue equal to the constant "
              u8"value.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((Constant<Int, 42>::kValue), 42);

        SYNTROPY_UNIT_EQUAL((BoolType<true>::kValue), true);
        SYNTROPY_UNIT_EQUAL((BoolType<false>::kValue), false);
        SYNTROPY_UNIT_EQUAL((IntType<11>::kValue), 11);

        SYNTROPY_UNIT_EQUAL((TrueType::kValue), true);
        SYNTROPY_UNIT_EQUAL((FalseType::kValue), false);
    })

    .TestCase(u8"Type list have member kCount equal to their rank.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((TypeList<Int, Float, Int>::kCount), 3);
    })

    .TestCase(u8"Type list have a member SelfType equal to their type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((TypeList<Int, Float>::SelfType),
                           (TypeList<Int, Float>));

    })

    .TestCase(u8"Type list have a member HeadType equal to the first element "
              u8"type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((TypeList<Int, Float>::HeadType), Int);
    })

    .TestCase(u8"Type list have a member RestTypes equal the type list except "
              u8"the first one.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((TypeList<Int, Float>::RestTypes),
                           (TypeList<Float>));
    })

    .TestCase(u8"AliasList is an alias for TypeLists.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((AliasList<Int, Float>::Type),
                           (TypeList<Int, Float>));
    })

    .TestCase(u8"AliasSequence is an alias for Sequences.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((AliasSequence<42, 11>::Type),
                           (Sequence<42, 11>));
    })

    .TestCase(u8"AlwaysFalse accepts any number of types and evaluated to "
              u8"false.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((AlwaysFalse<Int, Float>), false);
    })

    .TestCase(u8"Void accepts any number of types and is the same as void.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((Void<Int, Float>), void);
    });

}

// ===========================================================================
