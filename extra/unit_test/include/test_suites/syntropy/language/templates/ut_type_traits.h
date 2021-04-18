
/// \file ut_invoke.h
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/concepts.h"
#include "syntropy/language/templates/invoke.h"

#include "syntropy/diagnostics/unit_test/unit_test.h"

// ===========================================================================

namespace UnitTest
{
    /************************************************************************/
    /* TYPE TRAITS TEST FIXTURE                                             */
    /************************************************************************/

    /// \brief Type traits test fixture.
    struct TypeTraitsTestFixture
    {

    };

    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline auto& ut_type_traits
        = Syntropy::UnitTest::MakeAutoUnitTest<TypeTraitsTestFixture>(
            u8"type_traits.templates.language.syntropy")

    .TestCase(u8"IdentityOf is equal to the type itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((IdentityOf<ReadOnly<Int>>), (ReadOnly<Int>));
        SYNTROPY_UNIT_SAME((IdentityOf<ReadWrite<Int>>), (ReadWrite<Int>));
        SYNTROPY_UNIT_SAME((IdentityOf<Immutable<Int>>), (Immutable<Int>));
        SYNTROPY_UNIT_SAME((IdentityOf<Mutable<Int>>), (Mutable<Int>));
        SYNTROPY_UNIT_SAME((IdentityOf<Immovable<Int>>), (Immovable<Int>));
        SYNTROPY_UNIT_SAME((IdentityOf<Movable<Int>>), (Movable<Int>));
    })

    .TestCase(u8"IdentityOf of a type list is equal to the type list itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((IdentityOf<TypeList<ReadOnly<Int>,
                                                ReadWrite<Int>,
                                                Immutable<Int>,
                                                Mutable<Int>,
                                                Immovable<Int>,
                                                Movable<Int>>>),
                           (TypeList<ReadOnly<Int>,
                                     ReadWrite<Int>,
                                     Immutable<Int>,
                                     Mutable<Int>,
                                     Immovable<Int>,
                                     Movable<Int>>));
    })

    .TestCase(u8"UnqualifiedOf is equal to unqualified (read-write) "
              u8"value-type of a type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((UnqualifiedOf<ReadOnly<Int>>), (ReadWrite<Int>));
        SYNTROPY_UNIT_SAME((UnqualifiedOf<ReadWrite<Int>>), (ReadWrite<Int>));
        SYNTROPY_UNIT_SAME((UnqualifiedOf<Immutable<Int>>), (ReadWrite<Int>));
        SYNTROPY_UNIT_SAME((UnqualifiedOf<Mutable<Int>>), (ReadWrite<Int>));
        SYNTROPY_UNIT_SAME((UnqualifiedOf<Immovable<Int>>), (ReadWrite<Int>));
        SYNTROPY_UNIT_SAME((UnqualifiedOf<Movable<Int>>), (ReadWrite<Int>));
    })

    .TestCase(u8"UnqualifiedOf of a type list is equal to a type list of "
              u8"unqualifed (read-write) value-types of each element.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((UnqualifiedOf<TypeList<ReadOnly<Int>,
                                                   ReadWrite<Int>,
                                                   Immutable<Int>,
                                                   Mutable<Int>,
                                                   Immovable<Int>,
                                                   Movable<Int>>>),
                           (TypeList<ReadWrite<Int>,
                                     ReadWrite<Int>,
                                     ReadWrite<Int>,
                                     ReadWrite<Int>,
                                     ReadWrite<Int>,
                                     ReadWrite<Int>>));
    })

    .TestCase(u8"QualifiedOf is equal to qualified (read-write or read-only) "
              u8"value-type of a type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((QualifiedOf<ReadOnly<Int>>), (ReadOnly<Int>));
        SYNTROPY_UNIT_SAME((QualifiedOf<ReadWrite<Int>>), (ReadWrite<Int>));
        SYNTROPY_UNIT_SAME((QualifiedOf<Immutable<Int>>), (ReadOnly<Int>));
        SYNTROPY_UNIT_SAME((QualifiedOf<Mutable<Int>>), (ReadWrite<Int>));
        SYNTROPY_UNIT_SAME((QualifiedOf<Immovable<Int>>), (ReadOnly<Int>));
        SYNTROPY_UNIT_SAME((QualifiedOf<Movable<Int>>), (ReadWrite<Int>));
    })

    .TestCase(u8"QualifiedOf of a type list is equal to a type list of "
              u8"qualified (read-write or read-only) value-types of each "
              u8"element.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((QualifiedOf<TypeList<ReadOnly<Int>,
                                                 ReadWrite<Int>,
                                                 Immutable<Int>,
                                                 Mutable<Int>,
                                                 Immovable<Int>,
                                                 Movable<Int>>>),
                           (TypeList<ReadOnly<Int>,
                                     ReadWrite<Int>,
                                     ReadOnly<Int>,
                                     ReadWrite<Int>,
                                     ReadOnly<Int>,
                                     ReadWrite<Int>>));
    })

    .TestCase(u8"ReadOnlyOf is equal to the read-only value-types of a type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ReadOnlyOf<ReadOnly<Int>>), (ReadOnly<Int>));
        SYNTROPY_UNIT_SAME((ReadOnlyOf<ReadWrite<Int>>), (ReadOnly<Int>));
        SYNTROPY_UNIT_SAME((ReadOnlyOf<Immutable<Int>>), (ReadOnly<Int>));
        SYNTROPY_UNIT_SAME((ReadOnlyOf<Mutable<Int>>), (ReadOnly<Int>));
        SYNTROPY_UNIT_SAME((ReadOnlyOf<Immovable<Int>>), (ReadOnly<Int>));
        SYNTROPY_UNIT_SAME((ReadOnlyOf<Movable<Int>>), (ReadOnly<Int>));
    })

    .TestCase(u8"ReadOnlyOf of a type list is equal to a type list of the"
              u8"read-only value-types of each element.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ReadOnlyOf<TypeList<ReadOnly<Int>,
                                                ReadWrite<Int>,
                                                Immutable<Int>,
                                                Mutable<Int>,
                                                Immovable<Int>,
                                                Movable<Int>>>),
                           (TypeList<ReadOnly<Int>,
                                     ReadOnly<Int>,
                                     ReadOnly<Int>,
                                     ReadOnly<Int>,
                                     ReadOnly<Int>,
                                     ReadOnly<Int>>));
    })

    .TestCase(u8"ReadWriteOf is equal to the read-write value-type of a type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ReadWriteOf<ReadOnly<Int>>), (ReadWrite<Int>));
        SYNTROPY_UNIT_SAME((ReadWriteOf<ReadWrite<Int>>), (ReadWrite<Int>));
        SYNTROPY_UNIT_SAME((ReadWriteOf<Immutable<Int>>), (ReadWrite<Int>));
        SYNTROPY_UNIT_SAME((ReadWriteOf<Mutable<Int>>), (ReadWrite<Int>));
        SYNTROPY_UNIT_SAME((ReadWriteOf<Immovable<Int>>), (ReadWrite<Int>));
        SYNTROPY_UNIT_SAME((ReadWriteOf<Movable<Int>>), (ReadWrite<Int>));
    })

    .TestCase(u8"ReadWriteOf of a type list is equal to a type list of the"
              u8"read-write value-types of each element.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ReadWriteOf<TypeList<ReadOnly<Int>,
                                                 ReadWrite<Int>,
                                                 Immutable<Int>,
                                                 Mutable<Int>,
                                                 Immovable<Int>,
                                                 Movable<Int>>>),
                           (TypeList<ReadWrite<Int>,
                                     ReadWrite<Int>,
                                     ReadWrite<Int>,
                                     ReadWrite<Int>,
                                     ReadWrite<Int>,
                                     ReadWrite<Int>>));
    })

    .TestCase(u8"ImmutableOf is equal to the immutable reference-type of a "
              u8"type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ImmutableOf<ReadOnly<Int>>), (Immutable<Int>));
        SYNTROPY_UNIT_SAME((ImmutableOf<ReadWrite<Int>>), (Immutable<Int>));
        SYNTROPY_UNIT_SAME((ImmutableOf<Immutable<Int>>), (Immutable<Int>));
        SYNTROPY_UNIT_SAME((ImmutableOf<Mutable<Int>>), (Immutable<Int>));
        SYNTROPY_UNIT_SAME((ImmutableOf<Immovable<Int>>), (Immutable<Int>));
        SYNTROPY_UNIT_SAME((ImmutableOf<Movable<Int>>), (Immutable<Int>));
    })

    .TestCase(u8"ImmutableOf of a type list is equal to a type list of the"
              u8"immutable reference-types of each element.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ImmutableOf<TypeList<ReadOnly<Int>,
                                                 ReadWrite<Int>,
                                                 Immutable<Int>,
                                                 Mutable<Int>,
                                                 Immovable<Int>,
                                                 Movable<Int>>>),
                           (TypeList<Immutable<Int>,
                                     Immutable<Int>,
                                     Immutable<Int>,
                                     Immutable<Int>,
                                     Immutable<Int>,
                                     Immutable<Int>>));
    })

    .TestCase(u8"MutableOf is equal to the mutable reference-type of a "
              u8"type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((MutableOf<ReadOnly<Int>>), (Mutable<Int>));
        SYNTROPY_UNIT_SAME((MutableOf<ReadWrite<Int>>), (Mutable<Int>));
        SYNTROPY_UNIT_SAME((MutableOf<Immutable<Int>>), (Mutable<Int>));
        SYNTROPY_UNIT_SAME((MutableOf<Mutable<Int>>), (Mutable<Int>));
        SYNTROPY_UNIT_SAME((MutableOf<Immovable<Int>>), (Mutable<Int>));
        SYNTROPY_UNIT_SAME((MutableOf<Movable<Int>>), (Mutable<Int>));
    })

    .TestCase(u8"MutableOf of a type list is equal to a type list of the"
              u8"mutable reference-types of each element.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((MutableOf<TypeList<ReadOnly<Int>,
                                               ReadWrite<Int>,
                                               Immutable<Int>,
                                               Mutable<Int>,
                                               Immovable<Int>,
                                               Movable<Int>>>),
                           (TypeList<Mutable<Int>,
                                     Mutable<Int>,
                                     Mutable<Int>,
                                     Mutable<Int>,
                                     Mutable<Int>,
                                     Mutable<Int>>));
    })

    .TestCase(u8"ImmovableOf is equal to the immovable reference-type of a "
              u8"type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ImmovableOf<ReadOnly<Int>>), (Immovable<Int>));
        SYNTROPY_UNIT_SAME((ImmovableOf<ReadWrite<Int>>), (Immovable<Int>));
        SYNTROPY_UNIT_SAME((ImmovableOf<Immutable<Int>>), (Immovable<Int>));
        SYNTROPY_UNIT_SAME((ImmovableOf<Mutable<Int>>), (Immovable<Int>));
        SYNTROPY_UNIT_SAME((ImmovableOf<Immovable<Int>>), (Immovable<Int>));
        SYNTROPY_UNIT_SAME((ImmovableOf<Movable<Int>>), (Immovable<Int>));
    })

    .TestCase(u8"ImmovableOf of a type list is equal to a type list of the"
              u8"immovable reference-type of each element.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ImmovableOf<TypeList<ReadOnly<Int>,
                                                 ReadWrite<Int>,
                                                 Immutable<Int>,
                                                 Mutable<Int>,
                                                 Immovable<Int>,
                                                 Movable<Int>>>),
                           (TypeList<Immovable<Int>,
                                     Immovable<Int>,
                                     Immovable<Int>,
                                     Immovable<Int>,
                                     Immovable<Int>,
                                     Immovable<Int>>));
    })

    .TestCase(u8"MovableOf is equal to the movable reference-type of a "
              u8"type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((MovableOf<ReadOnly<Int>>), (Movable<Int>));
        SYNTROPY_UNIT_SAME((MovableOf<ReadWrite<Int>>), (Movable<Int>));
        SYNTROPY_UNIT_SAME((MovableOf<Immutable<Int>>), (Movable<Int>));
        SYNTROPY_UNIT_SAME((MovableOf<Mutable<Int>>), (Movable<Int>));
        SYNTROPY_UNIT_SAME((MovableOf<Immovable<Int>>), (Movable<Int>));
        SYNTROPY_UNIT_SAME((MovableOf<Movable<Int>>), (Movable<Int>));
    })

    .TestCase(u8"MovableOf of a type list is equal to a type list of the"
              u8"movable reference-type of each element.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((MovableOf<TypeList<ReadOnly<Int>,
                                               ReadWrite<Int>,
                                               Immutable<Int>,
                                               Mutable<Int>,
                                               Immovable<Int>,
                                               Movable<Int>>>),
                           (TypeList<Movable<Int>,
                                     Movable<Int>,
                                     Movable<Int>,
                                     Movable<Int>,
                                     Movable<Int>,
                                     Movable<Int>>));
    })

    .TestCase(u8"ReferenceOf is equal to either the immutable or mutable "
              u8"reference-type of a type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ReferenceOf<ReadOnly<Int>>), (Immutable<Int>));
        SYNTROPY_UNIT_SAME((ReferenceOf<ReadWrite<Int>>), (Mutable<Int>));
        SYNTROPY_UNIT_SAME((ReferenceOf<Immutable<Int>>), (Immutable<Int>));
        SYNTROPY_UNIT_SAME((ReferenceOf<Mutable<Int>>), (Mutable<Int>));
        SYNTROPY_UNIT_SAME((ReferenceOf<Immovable<Int>>), (Immutable<Int>));
        SYNTROPY_UNIT_SAME((ReferenceOf<Movable<Int>>), (Mutable<Int>));
    })

    .TestCase(u8"ReferenceOf of a type list is equal to a type list of either"
              u8"the immutable or mutable reference-types of each element.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ReferenceOf<TypeList<ReadOnly<Int>,
                                                 ReadWrite<Int>,
                                                 Immutable<Int>,
                                                 Mutable<Int>,
                                                 Immovable<Int>,
                                                 Movable<Int>>>),
                           (TypeList<Immutable<Int>,
                                     Mutable<Int>,
                                     Immutable<Int>,
                                     Mutable<Int>,
                                     Immutable<Int>,
                                     Mutable<Int>>));
    })

    .TestCase(u8"ForwardingOf forwards reference-types as themselves and "
              u8"value-types as either immovable or movable reference-types",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ForwardingOf<ReadOnly<Int>>), (Immovable<Int>));
        SYNTROPY_UNIT_SAME((ForwardingOf<ReadWrite<Int>>), (Movable<Int>));
        SYNTROPY_UNIT_SAME((ForwardingOf<Immutable<Int>>), (Immutable<Int>));
        SYNTROPY_UNIT_SAME((ForwardingOf<Mutable<Int>>), (Mutable<Int>));
        SYNTROPY_UNIT_SAME((ForwardingOf<Immovable<Int>>), (Immovable<Int>));
        SYNTROPY_UNIT_SAME((ForwardingOf<Movable<Int>>), (Movable<Int>));
    })

    .TestCase(u8"ForwardingOf of a type list is equal to a type list of either"
              u8"the forwarded reference-types of each element or either "
              u8"the immovable or movable reference-types of value-types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ForwardingOf<TypeList<ReadOnly<Int>,
                                                  ReadWrite<Int>,
                                                  Immutable<Int>,
                                                  Mutable<Int>,
                                                  Immovable<Int>,
                                                  Movable<Int>>>),
                           (TypeList<Immovable<Int>,
                                     Movable<Int>,
                                     Immutable<Int>,
                                     Mutable<Int>,
                                     Immovable<Int>,
                                     Movable<Int>>));
    })

    .TestCase(u8"TypeListOf of an empty list is the empty type list.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((TypeListOf<>), (TypeList<>));
    })

    .TestCase(u8"TypeListOf of a single type list is the type list itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((TypeListOf<TypeList<Int, Float>>),
                           (TypeList<Int, Float>));
    })

    .TestCase(u8"TypeListOf of a non-type list type is a type list with a "
              u8"single element of type type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((TypeListOf<Int>), (TypeList<Int>));
    })

    .TestCase(u8"TypeListOf of two or more type lists concatenates the "
              u8"type lists one after the other.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((TypeListOf<TypeList<Int, Float>, TypeList<Int>>),
                           (TypeList<Int, Float, Int>));
    })

    .TestCase(u8"TypeListOf flattens all type lists into a single type list.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((TypeListOf<TypeList<Int, TypeList<Float, Float>>,
                                       TypeList<Int>,
                                       TypeList<TypeList<TypeList<Bool>>>>),
                           (TypeList<Int, Float, Float, Int, Bool>));
    })

    .TestCase(u8"HeadTypeOf of a parameter pack is equal to the first type in "
              u8"the parameter pack.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((HeadTypeOf<Int, Float, Float>), (Int));
    })

    .TestCase(u8"HeadTypeOf of a type list is equal to the first type in "
              u8"the type list.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((HeadTypeOf<TypeList<Int, Float, Float>>), (Int));
    })

    .TestCase(u8"RestTypeOf of a parameter pack is equal to the type list "
              u8"with all types in the parameter pack except the first one.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((RestTypeOf<Int, Float, Float>),
                           (TypeList<Float, Float>));
    })

    .TestCase(u8"RestTypeOf of a type list is equal to the type list "
              u8"with all types in the type list except the first one.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((RestTypeOf<TypeList<Int, Float, Float>>),
                           (TypeList<Float, Float>));
    })

    .TestCase(u8"ElementTypeOf provides indexed access to a parameter pack's "
              u8"types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ElementTypeOf<1, Int, Float, Bool>), (Float));
    })

    .TestCase(u8"ElementTypeOf provides indexed access to a type list's "
              u8"types.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((ElementTypeOf<1, TypeList<Int, Float, Bool>>),
                           (Float));
    })

    .TestCase(u8"ElementCountOf a parameter pack is equal to the number of "
              u8"elements in the parameter pack.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((ElementCountOf<Int, Float, Bool>), 3);
    })

    .TestCase(u8"ElementCountOf a type list is equal to the number of "
              u8"elements in the type list.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((ElementCountOf<TypeList<Int, Float, Bool>>), 3);
    })

    .TestCase(u8"ElementIndexOf is equal to the index of the first type in a "
              u8"parameter pack matching a given type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((ElementIndexOf<Bool, Int, Float, Bool>), 2);
    })

    .TestCase(u8"ElementIndexOf is equal to the index of the first type in a "
              u8"type list matching a given type.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_EQUAL((ElementIndexOf<Bool, TypeList<Int, Float, Bool>>),
                            2);
    })

    .TestCase(u8"MakeSequence generates a increasing sequence of integers "
              u8"starting from 0 with a given number of elements.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((MakeSequence<4>), (Sequence<0, 1, 2, 3>));
    })

    .TestCase(u8"SequenceFor generates a increasing sequence of integers "
              u8"starting from 0 with the same number of elements in a "
              u8"parameter pack.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((SequenceFor<Int, Float, Bool>),
                           (Sequence<0, 1, 2>));
    })

    .TestCase(u8"SequenceFor generates a increasing sequence of integers "
              u8"starting from 0 with the same number of elements in a "
              u8"type list.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((SequenceFor<TypeList<Int, Float, Bool>>),
                           (Sequence<0, 1, 2>));
    })

    .TestCase(u8"SequenceFor of an empty parameter pack returns the empty "
              u8"sequence.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((SequenceFor<>), (Sequence<>));
    })

    .TestCase(u8"SequenceFor of an empty type list returns the empty"
              u8"sequence.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((SequenceFor<TypeList<>>), (Sequence<>));
    })

    .TestCase(u8"SequenceAdd increases or decreases each element in a "
              u8"by a given amount.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((SequenceAdd<2, Sequence<0, 1, 2>>),
                           (Sequence<2, 3, 4>));
    })

    .TestCase(u8"SequenceConcatenate of empty sequences is equal to the empty "
              u8"sequence.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((SequenceConcatenate<>),
                           (Sequence<>));

        SYNTROPY_UNIT_SAME((SequenceConcatenate<Sequence<>>),
                           (Sequence<>));

        SYNTROPY_UNIT_SAME((SequenceConcatenate<Sequence<>, Sequence<>>),
                           (Sequence<>));
    })

    .TestCase(u8"SequenceConcatenate of a single sequence is the sequence "
              u8"itself.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((SequenceConcatenate<Sequence<1, 2, 3>>),
                           (Sequence<1, 2, 3>));
    })

    .TestCase(u8"SequenceConcatenate concatenates one or more sequences "
              u8"skipping empty ones.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((SequenceConcatenate<Sequence<1, 2, 3>,
                                        Sequence<>,
                                        Sequence<4, 5>>),
                           (Sequence<1, 2, 3, 4, 5>));
    })

    .TestCase(u8"SequenceRepeat with 0 elements is equal to the empty "
              u8"sequence.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((SequenceRepeat<42, 0>), (Sequence<>));
    })

    .TestCase(u8"SequenceRepeat generates a sequence of repeating numbers "
              u8"with given size.",
    [](auto& fixture)
    {
        using namespace Syntropy;
        using namespace Syntropy::Templates;

        SYNTROPY_UNIT_SAME((SequenceRepeat<42, 3>), (Sequence<42, 42, 42>));
    });

}

// ===========================================================================
