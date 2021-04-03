
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
        : AliasListHelper<typename TTypeHelpers::Type...> {};

    //

    /// \brief Type identical to TType.
    template <typename TType>
    struct ExactOfHelper
        : Alias<TType> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct ExactOfHelper<TypeList<TTypes...>>
        : AliasListHelper<ExactOfHelper<TTypes>...> {};

    //

    /// \brief Qualifier-removing value type of TType.
    template <typename TType>
    struct UnqualifiedOfHelper
        :  Alias<std::remove_cvref_t<TType>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct UnqualifiedOfHelper<TypeList<TTypes...>>
        : AliasListHelper<UnqualifiedOfHelper<TTypes>...> {};

    //

    /// \brief Qualifier-preserving value type of TType.
    template <typename TType>
    struct QualifiedOfHelper
        : Alias<std::remove_reference_t<TType>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct QualifiedOfHelper<TypeList<TTypes...>>
        : AliasListHelper<QualifiedOfHelper<TTypes>...> {};

    //

    /// \brief Mutable reference type of TType.
    template <typename TType>
    struct MutableOfHelper
        : Alias<std::add_lvalue_reference_t<std::remove_cvref_t<TType>>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct MutableOfHelper<TypeList<TTypes...>>
        : AliasListHelper<MutableOfHelper<TTypes>...> {};

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

    //

    /// \brief Movable reference type of TType.
    template <typename TType>
    struct MovableOfHelper
        : Alias<std::add_rvalue_reference_t<std::remove_cvref_t<TType>>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct MovableOfHelper<TypeList<TTypes...>>
        : AliasListHelper<MovableOfHelper<TTypes>...> {};

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

    //

    /// \brief Either mutable or immutable reference type of TType.
    template <typename TType>
    struct ReferenceOfHelper
        : Alias<std::add_lvalue_reference_t<TType>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct ReferenceOfHelper<TypeList<TTypes...>>
        : AliasListHelper<ReferenceOfHelper<TTypes>...> {};

    //

    /// \brief Forwarding reference type of TType.
    ///
    /// \remarks This transform honors reference collapsing rule.
    template <typename TType>
    struct ForwardingOfHelper
        : Alias<std::add_rvalue_reference_t<TType>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct ForwardingOfHelper<TypeList<TTypes...>>
        : AliasListHelper<ForwardingOfHelper<TTypes>...> {};

    //

    /// \brief Function pointer type.
    template <typename TType>
    struct FunctionOfHelper
        : Alias<std::add_pointer_t<TType>> {};

    /// \brief Partial template specialization for type lists.
    template <typename... TTypes>
    struct FunctionOfHelper<TypeList<TTypes...>>
        : AliasListHelper<FunctionOfHelper<TTypes>...> {};

    /************************************************************************/
    /* TYPE LIST TRAITS                                                     */
    /************************************************************************/

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
    inline constexpr Int ElementCountOf
        = sizeof...(TTypes);

    /// \brief Specialization for type lists
    template <typename... TTypes>
    inline constexpr Int ElementCountOf<TypeList<TTypes...>>
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
        : IntConstant<TIndex> {};

    /// \brief Index of the first element in a type list or parameter pack with
    ///        a given type.
    template <typename TType, typename TTypes>
    inline constexpr Int ElementIndexOf
        = ElementIndexOfHelper<0, TType, TTypes>::kValue;

    //

}

// ===========================================================================
