
/// \file traits_details.h
///
/// \author Raffaele D. Facendola - November 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/templates/templates.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* TYPE TRANSFORM                                                       */
    /************************************************************************/

    /// \brief Binds together multiple type helpers in a single list.
    template <typename... TTypeHelpers>
    struct AliasListHelper
        : AliasList<typename TTypeHelpers::Type...> {};

    //

    /// \brief Type identical to TType.
    template <typename TType>
    struct IdentityOfHelper
        : Alias<TType> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct IdentityOfHelper<TypeList<TTypes...>>
        : AliasListHelper<IdentityOfHelper<TTypes>...> {};

    /// \brief Type identical to TType.
    template <typename TType>
    using IdentityOf
        = typename IdentityOfHelper<TType>::Type;

    //

    /// \brief Qualifier-removing value type of TType.
    template <typename TType>
    struct UnqualifiedOfHelper
        :  Alias<std::remove_cvref_t<TType>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct UnqualifiedOfHelper<TypeList<TTypes...>>
        : AliasListHelper<UnqualifiedOfHelper<TTypes>...> {};

    /// \brief Qualifier-removing value type of TType.
    template <typename TType>
    using UnqualifiedOf
        = typename UnqualifiedOfHelper<TType>::Type;

    //

    /// \brief Qualifier-preserving value type of TType.
    template <typename TType>
    struct QualifiedOfHelper
        : Alias<std::remove_reference_t<TType>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct QualifiedOfHelper<TypeList<TTypes...>>
        : AliasListHelper<QualifiedOfHelper<TTypes>...> {};

    /// \brief Qualifier-preserving value type of TType.
    template <typename TType>
    using QualifiedOf
        = typename QualifiedOfHelper<TType>::Type;

    //

    /// \brief Read-only value-type of TType.
    template <typename TType>
    struct ReadOnlyOfHelper
        : Alias<std::add_const_t<UnqualifiedOf<TType>>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct ReadOnlyOfHelper<TypeList<TTypes...>>
        : AliasListHelper<ReadOnlyOfHelper<TTypes>...> {};

    /// \brief Read-only value-type of TType.
    template <typename TType>
    using ReadOnlyOf
        = typename ReadOnlyOfHelper<TType>::Type;

    //

    /// \brief Read-write value-type of TType.
    template <typename TType>
    struct ReadWriteOfHelper
        : Alias<UnqualifiedOf<TType>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct ReadWriteOfHelper<TypeList<TTypes...>>
        : AliasListHelper<ReadWriteOfHelper<TTypes>...> {};

    /// \brief Read-write value-type of TType.
    template <typename TType>
    using ReadWriteOf
        = typename ReadWriteOfHelper<TType>::Type;

    //

    /// \brief Immutable reference type of TType.
    template <typename TType>
    struct ImmutableOfHelper
        : Alias<std::add_lvalue_reference_t<
            std::add_const_t<std::remove_cvref_t<TType>>>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct ImmutableOfHelper<TypeList<TTypes...>>
        : AliasListHelper<ImmutableOfHelper<TTypes>...> {};

    /// \brief Immutable reference type of TType.
    template <typename TType>
    using ImmutableOf
        = typename ImmutableOfHelper<TType>::Type;
    //

    /// \brief Mutable reference type of TType.
    template <typename TType>
    struct MutableOfHelper
        : Alias<std::add_lvalue_reference_t<std::remove_cvref_t<TType>>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct MutableOfHelper<TypeList<TTypes...>>
        : AliasListHelper<MutableOfHelper<TTypes>...> {};

    /// \brief Mutable reference type of TType.
    template <typename TType>
    using MutableOf
        = typename MutableOfHelper<TType>::Type;

    //

    /// \brief Immovable reference type of TType.
    template <typename TType>
    struct ImmovableOfHelper
        : Alias<std::add_rvalue_reference_t<
            std::add_const_t<std::remove_cvref_t<TType>>>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct ImmovableOfHelper<TypeList<TTypes...>>
        : AliasListHelper<ImmovableOfHelper<TTypes>...> {};

    /// \brief Immovable reference type of TType.
    template <typename TType>
    using ImmovableOf
        = typename ImmovableOfHelper<TType>::Type;

    //

    /// \brief Movable reference type of TType.
    template <typename TType>
    struct MovableOfHelper
        : Alias<std::add_rvalue_reference_t<std::remove_cvref_t<TType>>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct MovableOfHelper<TypeList<TTypes...>>
        : AliasListHelper<MovableOfHelper<TTypes>...> {};

    /// \brief Movable reference type of TType.
    template <typename TType>
    using MovableOf
        = typename MovableOfHelper<TType>::Type;

    //

    /// \brief Either mutable or immutable reference type of TType.
    template <typename TType>
    struct ReferenceOfHelper
        : Alias<std::add_lvalue_reference_t<TType>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct ReferenceOfHelper<TypeList<TTypes...>>
        : AliasListHelper<ReferenceOfHelper<TTypes>...> {};

    /// \brief Either mutable or immutable reference type of TType.
    template <typename TType>
    using ReferenceOf
        = typename ReferenceOfHelper<TType>::Type;

    //

    /// \brief Forwarding reference type of TType.
    template <typename TType>
    struct ForwardingOfHelper
        : Alias<std::add_rvalue_reference_t<TType>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct ForwardingOfHelper<TypeList<TTypes...>>
        : AliasListHelper<ForwardingOfHelper<TTypes>...> {};

    /// \brief Forwarding reference type of TType.
    template <typename TType>
    using ForwardingOf
        = typename ForwardingOfHelper<TType>::Type;

    /************************************************************************/
    /* TYPE LIST TRAITS                                                     */
    /************************************************************************/

    //

    /// \brief Concatenate all types in a single type list.
    template <typename TTypes, typename UTypes>
    struct TypeListOfHelper{};

    /// \brief Specialization for empty type lists. End of recursion.
    template <typename... TTypes>
    struct TypeListOfHelper<TypeList<>, TypeList<TTypes...>>
        : AliasList<TTypes...> {};

    /// \brief Append the non-type list head of the first list to the second.
    template <typename TType, typename... TTypes, typename... UTypes>
    struct TypeListOfHelper<TypeList<TType, TTypes...>,
                            TypeList<UTypes...>>
        : TypeListOfHelper<TypeList<TTypes...>,
                           TypeList<UTypes..., TType>> {};

    /// \brief Specialization for type lists: Expands the type list head.
    template <typename... TTypes, typename... UTypes, typename... VTypes>
    struct TypeListOfHelper<TypeList<TypeList<TTypes...>,
                                     UTypes...>,
                            TypeList<VTypes...>>
        : TypeListOfHelper<TypeList<TTypes..., UTypes...>,
                           TypeList<VTypes...>> {};

    /// \brief Concatenate all types in a single type list.
    template <typename... TTypes>
    using TypeListOf
        = typename TypeListOfHelper<TypeList<TTypes...>, TypeList<>>::Type;

    //

    /// \brief Type of the first element in a parameter pack.
    template <typename TType, typename... TTypes>
    struct HeadTypeOfHelper
        : Alias<TType> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct HeadTypeOfHelper<TypeList<TTypes...>>
        : Alias<typename TypeList<TTypes...>::HeadType> {};

    /// \brief Type of the first element in a type list or parameter pack.
    template <typename... TTypes>
    using HeadTypeOf
        = typename HeadTypeOfHelper<TTypes...>::Type;

    //

    /// \brief Discard the first element in a type list and return the
    ///        remaining elements.
    template <typename TType, typename... TTypes>
    struct RestTypeOfHelper
        : AliasList<TTypes...> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct RestTypeOfHelper<TypeList<TTypes...>>
        : Alias<typename TypeList<TTypes...>::RestTypes> {};

    /// \brief Drops the first element in a type list or parameter pack and
    ///        return a type list to the remaining ones.
    template <typename... TTypes>
    using RestTypeOf
        = typename RestTypeOfHelper<TTypes...>::Type;

    //

    /// \brief Discards elements until the index drops to zero or the list is
    ///        exhausted.
    template <Int TIndex, typename... TTypes>
    struct ElementTypeOfPackHelper
        : ElementTypeOfPackHelper<TIndex - 1, RestTypeOf<TTypes...>> {};

    /// \brief End of recursion.
    template <typename... TTypes>
    struct ElementTypeOfPackHelper<0, TTypes...>
        : Alias<HeadTypeOf<TTypes...>> {};

    /// \brief Helper type for parameters pack.
    /// \remarks This helper type makes sure not to flatten type lists other
    ///          than the top-most one.
    template <Int TIndex, typename... TTypes>
    struct ElementTypeOfHelper
        : ElementTypeOfPackHelper<TIndex, TTypes...> {};

    /// \brief Partial template specialization for type lists.
    template <Int TIndex, typename... TTypes>
    struct ElementTypeOfHelper<TIndex, TypeList<TTypes...>>
        : ElementTypeOfPackHelper<TIndex, TTypes...> {};

    /// \brief Type of a type list or parameter pack element, by index.
    template <Int TIndex, typename... TTypes>
    using ElementTypeOf
        = typename ElementTypeOfHelper<TIndex, TTypes...>::Type;

    //

    /// \brief Number of elements in a parameter pack.
    template <typename... TTypes>
    inline constexpr
    Int ElementCountOf
        = sizeof...(TTypes);

    /// \brief Specialization for type lists
    template <typename... TTypes>
    inline constexpr
    Int ElementCountOf<TypeList<TTypes...>>
        = sizeof...(TTypes);

    //

    /// \brief Discards elements until the first element is equal to TType or
    ///        the list is exhausted.
    template <Int TIndex, typename TType, typename... TTypes>
    struct ElementIndexOfPackHelper
        : ElementIndexOfPackHelper<TIndex + 1, TType, RestTypeOf<TTypes...>> {};

    /// \brief End of recursion.
    template <Int TIndex, typename... TTypes>
    struct ElementIndexOfPackHelper<TIndex, HeadTypeOf<TTypes...>, TTypes...>
        : IntType<TIndex> {};

    /// \brief Helper type for parameters pack.
    /// \remarks This helper type makes sure not to flatten type lists other
    ///          than the top-most one.
    template <typename TType, typename... TTypes>
    struct ElementIndexOfHelper
        : ElementIndexOfPackHelper<0, TType, TTypes...> {};

    /// \brief Partial template specialization for type lists.
    template <typename TType, typename... TTypes>
    struct ElementIndexOfHelper<TType, TypeList<TTypes...>>
        : ElementIndexOfPackHelper<0, TType, TTypes...> {};

    /// \brief Index of the first element in a type list or parameter pack with
    ///        a given type.
    template <typename TType, typename... TTypes>
    inline constexpr
    Int ElementIndexOf
        = ElementIndexOfHelper<TType, TTypes...>::kValue;

    //

    /************************************************************************/
    /* SEQUENCE TRAITS                                                      */
    /************************************************************************/

    // MakeSequence.
    // =============

    /// \brief Generate a contiguous sequence of TCount integers, starting
    ///        from 0.
    template <Int TCount, Int... TSequence>
    struct MakeSequenceHelper
        : MakeSequenceHelper<TCount - 1, TCount - 1, TSequence...> {};

    /// \brief End of recursion.
    template <Int... TSequence>
    struct MakeSequenceHelper<0, TSequence...>
        : AliasSequence<TSequence...> {};

    /// \brief Generate a contiguous sequence of TCount integers, starting
    ///        from 0.
    template <Int TCount>
    using MakeSequence
        = typename MakeSequenceHelper<TCount>::Type;

    // SequenceFor.
    // ============

    /// \brief Generate a contiguous sequence of integers, starting
    ///        from 0 with a number of elements equal to the number of
    ///        elements in a parameter pack or type list.
    template <typename... TTypes>
    using SequenceFor
        = MakeSequence<ElementCountOf<TTypes...>>;

    // SequenceAdd.
    // ============

    /// \brief Add a value to each element in a sequence.
    template <Int TValue, typename TSequence>
    struct SequenceAddHelper {};

    /// \brief Add a value to each element in a sequence.
    template <Int TValue, Int... TIndices>
    struct SequenceAddHelper<TValue, Sequence<TIndices...>>
        : AliasSequence<(TIndices + TValue)...> {};

    /// \brief Add a value to each element in a sequence.
    template <Int TValue, typename TSequence>
    using SequenceAdd
        = typename SequenceAddHelper<TValue, TSequence>::Type;

    // SequenceConcatenate.
    // ============

    /// \brief Concatenate all elements in two or more sequences.
    template <typename TSequence, typename... USequences>
    struct SequenceConcatenateHelper{};

    /// \brief Specialization for single sequences. End of recursion.
    template <Int... TNumbers>
    struct SequenceConcatenateHelper<Sequence<TNumbers...>>
        : AliasSequence<TNumbers...> {};

    /// \brief Append the next sequence to the result.
    template <Int... TNumbers, Int... UNumbers, typename... TSequences>
    struct SequenceConcatenateHelper<Sequence<TNumbers...>,
                             Sequence<UNumbers...>,
                             TSequences...>
        : SequenceConcatenateHelper<Sequence<TNumbers..., UNumbers...>,
                            TSequences...> {};

    /// \brief Concatenate all sequences in a single sequence.
    template <typename... TSequences>
    using SequenceConcatenate
        = typename SequenceConcatenateHelper<Sequence<>, TSequences...>::Type;

    // SequenceRepeat.
    // ===============

    /// \brief Create a sequence of a repeating value.
    template <Int TValue, Int TCount, Int... TSequence>
    struct SequenceRepeatHelper
        : SequenceRepeatHelper<TValue, TCount - 1, TValue, TSequence...> {};

    /// \brief End of recursion.
    template <Int TValue, Int... TSequence>
    struct SequenceRepeatHelper<TValue, 0, TSequence...>
        : AliasSequence<TSequence...> {};

    /// \brief Create a sequence of a repeating value.
    template <Int TValue, Int TCount>
    using SequenceRepeat
        = typename SequenceRepeatHelper<TValue, TCount>::Type;

}

// ===========================================================================
