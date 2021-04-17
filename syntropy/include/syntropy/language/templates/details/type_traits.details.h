
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

    //

    /// \brief Function pointer type.
    template <typename TType>
    struct FunctionOfHelper
        : Alias<std::add_pointer_t<TType>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct FunctionOfHelper<TypeList<TTypes...>>
        : AliasListHelper<FunctionOfHelper<TTypes>...> {};

    /// \brief Function pointer type.
    template <typename TType>
    using FunctionOf
        = typename FunctionOfHelper<TType>::Type;

    /************************************************************************/
    /* TYPE LIST TRAITS                                                     */
    /************************************************************************/

    //

    /// \brief Concatenate all types in a single type list.
    template <typename... TTypes>
    struct TypeListOfHelper
        : AliasList<TTypes...> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes, typename... UTypes>
    struct TypeListOfHelper<TypeList<TTypes...>, UTypes...>
        : TypeListOfHelper<TTypes..., UTypes...> {};

    /// \brief Concatenate all types in a single type list.
    template <typename... TTypes>
    using TypeListOf
        = typename TypeListOfHelper<TTypes...>::Type;

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
    struct ElementTypeOfHelper
        : ElementTypeOfHelper<TIndex - 1, RestTypeOfHelper<TTypes...>> {};

    /// \brief End of recursion.
    template <typename... TTypes>
    struct ElementTypeOfHelper<0, TTypes...>
        : Alias<HeadTypeOf<TTypes...>> {};

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
    struct ElementIndexOfHelper
        : ElementIndexOfHelper<TIndex + 1, TType, RestTypeOf<TTypes...>> {};

    /// \brief End of recursion.
    template <Int TIndex, typename... TTypes>
    struct ElementIndexOfHelper<TIndex, HeadTypeOf<TTypes...>, TTypes...>
        : IntType<TIndex> {};

    /// \brief Index of the first element in a type list or parameter pack with
    ///        a given type.
    template <typename TType, typename TTypes>
    inline constexpr
    Int ElementIndexOf
        = ElementIndexOfHelper<0, TType, TTypes>::kValue;

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

    /// \brief Helper alias template used to convert a parameter pack to an
    ///        integer sequence of the same size.
    template <typename... TTypes>
    using SequenceFor
        = MakeSequence<sizeof...(TTypes)>;

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

    // SequenceCat.
    // ============

    /// \brief Concatenate one or more sequences together.
    template <typename... TSequences>
    struct SequenceCatHelper {};

    /// \brief Specialization for zero sequences.
    template <>
    struct SequenceCatHelper<>
        : AliasSequence<> {};

    /// \brief Specialization for one sequence.
    template <Int... TSequence>
    struct SequenceCatHelper<Sequence<TSequence...>>
        : AliasSequence<TSequence...> {};

    /// \brief Specialization for two or more sequences.
    template <Int... TSequence, Int... USequence, typename... TSequences>
    struct SequenceCatHelper<Sequence<TSequence...>,
                             Sequence<USequence...>,
                             TSequences...>
        : SequenceCatHelper<Sequence<TSequence..., USequence...>,
                            TSequences...> {};

    /// \brief Concatenate zero or more sequences together.
    template <typename... TSequences>
    using SequenceCat
        = typename SequenceCatHelper<TSequences...>::Type;

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
