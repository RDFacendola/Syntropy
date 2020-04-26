
/// \file set.h
/// \brief This header is part of the syntropy containers. It contains wrappers for sets and multisets.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <unordered_set>
#include <utility>
#include <functional>

#include "syntropy/type_traits.h"

#include "syntropy/allocators/polymorphic_allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* SET <TKEY, THASH, TPRED>                                             */
    /************************************************************************/

    /// \brief Alias type for std::unordered_set with polymorphic allocator type.
    template <typename TKey, typename THash = std::hash<TKey>, typename TPred = std::equal_to<TKey>>
    using Set = std::unordered_set<TKey, THash, TPred, PolymorphicAllocator<TKey>>;

    /************************************************************************/
    /* MULTISET <TKEY, THASH, TPRED>                                        */
    /************************************************************************/

    /// \brief Alias type for std::unordered_multiset with polymorphic allocator type.
    template <typename TKey, typename THash = std::hash<TKey>, typename TPred = std::equal_to<TKey>>
    using Multiset = std::unordered_multiset<TKey, THash, TPred, PolymorphicAllocator<TKey>>;

    /************************************************************************/
    /* TRAITS                                                               */
    /************************************************************************/

    /// \brief If TType represents a set provides the members constant value equal to true, otherwise value is false.
    /// \author Raffaele D. Facendola - November 2016
    template <typename TType>
    struct is_set : std::false_type {};

    /// \brief Specialization for sets.
    template <typename TType>
    struct is_set<Set<TType>> : std::true_type {};

    /// \brief Specialization for multiset.
    template <typename TType>
    struct is_set<Multiset<TType>> : std::true_type {};

    /// \brief Helper value for is_set<TType>.
    template <typename TType>
    constexpr bool is_set_v = is_set<TType>::value;

}
