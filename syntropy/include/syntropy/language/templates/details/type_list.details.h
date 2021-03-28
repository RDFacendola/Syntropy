
/// \file traits_details.h
///
/// \author Raffaele D. Facendola - March 2021.

#pragma once

#include "syntropy/language/foundation/types.h"

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* TYPE LIST                                                            */
    /************************************************************************/

    // HeadOf.
    // ========

    /// \brief Type of the first element in a type list.
    template <typename TTypes>
    using HeadOf = typename TTypes::FirstType;

    // RestOf.
    // ========

    /// \brief Discard the first element in a type lista and return the
    ///        remaining elements.
    template <typename TTypes>
    using RestOf = typename TTypes::RestType;

    // IndexOf.
    // ========

    /// \brief Discards elements until the first element is equal to TType or
    ///        the list is exhausted.
    template <Int TIndex, typename TType, typename TTypes, bool TStop>
    struct IndexOfHelper : IndexOfHelper<TIndex + 1,
                                         TType,
                                         RestOf<TTypes>,
                                         IsSame<HeadOf<TTypes>, TType>> {};

    /// \brief End of recursion.
    template <Int TIndex, typename TType, typename TTypes>
    struct IndexOfHelper<TIndex, TType, TTypes, true>
    {
        static constexpr TType kValue = TIndex;
    };

    /// \brief Index of the first element of type TType in TTypes.
    template <typename TType, typename TTypes>
    inline constexpr Int
    IndexOf = IndexOfHelper<0,
                            TType,
                            TTypes,
                            IsSame<HeadOf<TTypes>, TType>>::kValue;

    // ElementOf.
    // ==========

    /// \brief Discards elements until the index drops to zero or the list is
    ///        exhausted.
    template <Int TIndex, typename TTypes>
    struct ElementOfHelper : ElementOfHelper<TIndex - 1,
                                             RestOf<TTypes>> {};

    /// \brief End of recursion.
    template <typename TTypes>
    struct ElementOfHelper<0, TTypes>
    {
        using Type = HeadOf<TTypes>;
    };

    /// \brief Type of list element, by index.
    template <Int TIndex, typename TTypes>
    using ElementOf = typename ElementOfHelper<TIndex, TTypes>::Type;

}

// ===========================================================================
