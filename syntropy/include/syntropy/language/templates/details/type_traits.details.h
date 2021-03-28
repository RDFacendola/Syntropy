
/// \file traits_details.h
///
/// \author Raffaele D. Facendola - November 2020.

#pragma once

#include <type_traits>

#include "syntropy/language/foundation/types.h"

#include "syntropy/language/templates/templates.h"
#include "syntropy/language/templates/diagnostics.h"
#include "syntropy/language/templates/sfinae.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    /// \brief Get the numer of elements in a type list.
    template <typename TTypeList>
    inline constexpr Int
    CountOf = IllFormed<Int, TTypeList>::kValue;

    /// \brief Specialization for type lists.
    template <typename... TTypes>
    inline constexpr Int CountOf<Syntropy::Templates::TypeList<TTypes...>>
        = sizeof...(TTypes);

    // =======================================================================

    /// \brief Discards elements from UTypes until the first element is equal
    ///        to TType or the list is exhausted.
    template <typename TType, typename TFirst, typename... UTypes>
    struct IndexOfHelper : IndexOfHelper<TType, UTypes...> {};

    /// \brief Specialization for type lists starting with TType.
    template <typename TType, typename... UTypes>
    struct IndexOfHelper<TType, TType, UTypes...>
        : Constant<Int, sizeof...(UTypes)> {};

    /// \brief Index of the first occurrence of TType in TTypeList.
    template <typename TType, typename TTypeList>
    inline constexpr Int
    IndexOf = IllFormed<Int, TType, TTypeList>::kValue;

    /// \brief Specialization for type lists.
    template <typename TType, typename... UTypes>
    inline constexpr Int
    IndexOf<TType, TypeList<UTypes...>>
         = sizeof...(UTypes)
         - IndexOfHelper<TType, UTypes...>::kValue - 1;

    // =======================================================================

    /// \brief Get the type of the TIndex-th element in TTypeList.
    template <Int TIndex, typename TTypeList>
    struct ElementOfHelper;

    /// \brief Specialization for type lists. Recursive.
    template <Int TIndex, typename TElement, typename... TElements>
    struct ElementOfHelper<TIndex, TypeList<TElement, TElements...>>
        : ElementOfHelper<TIndex - 1, TypeList<TElements...>>
    {

    };

    /// \brief End of recursion.
    template <typename TElement, typename... TElements>
    struct ElementOfHelper<0, TypeList<TElement, TElements...>>
    {
        using Type = TElement;
    };

    /// \brief Get the type of the TIndex-th element in TTypeList.
    template <Int TIndex, typename TTypeList>
    using ElementOf = typename ElementOfHelper<TIndex, TTypeList>::Type;

    /************************************************************************/
    /* TYPE TRANSFORM                                                       */
    /************************************************************************/

    /// \brief Obtain the very same type of Type, establishing a non-deduced
    ///        context during template argument deduction.
    template <typename TType>
    using ExactOf = typename Alias<TType>::Type;

    /// \brief Obtain the value-type of TType without qualifiers.
    template <typename TType>
    using UnqualifiedOf = std::remove_cvref_t<TType>;

    /// \brief Obtain the value-type of TType preserving existing qualifiers.
    template <typename TType>
    using QualifiedOf = std::remove_reference_t<TType>;

    /// \brief Obtain a reference-type to a mutable instance of TType.
    template <typename TType>
    using MutableOf
        = std::add_lvalue_reference_t<UnqualifiedOf<TType>>;

    /// \brief Obtain a reference-type to an immutable instance of TType.
    template <typename TType>
    using ImmutableOf
        = std::add_lvalue_reference_t<std::add_const_t<UnqualifiedOf<TType>>>;

    /// \brief Obtain a reference-type to a mutable instance of TType whose
    ///        resources can be efficiently moved to another instance.
    template <typename TType>
    using MovableOf
        = std::add_rvalue_reference_t<UnqualifiedOf<TType>>;

    /// \brief Obtain a reference-type to an immutable instance of TType whose
    ///        resources can be efficiently moved to another instance.
    template <typename TType>
    using ImmovableOf
        = std::add_rvalue_reference_t<std::add_const_t<UnqualifiedOf<TType>>>;

    /// \brief Obtain the reference-type of TType preserving existing
    ///        qualifiers.
    template <typename TType>
    using ReferenceOf = std::add_lvalue_reference_t<TType>;

    /// \brief Obtain the forwarding-reference-type of TType preserving
    ///        existing qualifiers.
    ///
    /// This transform honors reference collapse rule.
    template <typename TType>
    using ForwardingOf = std::add_rvalue_reference_t<TType>;


    /// \brief Convert a function type to a function pointer.
    template <typename TFunction>
    using FunctionOf = std::add_pointer_t<TFunction>;

}

// ===========================================================================
