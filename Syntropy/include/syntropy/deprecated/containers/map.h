
/// \file map.h
/// \brief This header is part of the syntropy containers. It contains wrappers for maps and multimaps.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <map>
#include <unordered_map>
#include <utility>
#include <functional>

#include "syntropy/allocators/polymorphic_allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* MAP <TKEY, TVALUE, THASH, TPRED>                                     */
    /************************************************************************/

    /// \brief Alias type for std::unordered_map with polymorphic allocator type.
    template <typename TKey, typename TValue, typename THash = std::hash<TKey>, typename TPred = std::equal_to<TKey>>
    using Map = std::unordered_map<TKey, TValue, THash, TPred, PolymorphicAllocator<std::pair<const TKey, TValue>>>;

    /************************************************************************/
    /* MULTIMAP <TKEY, TVALUE, THASH, TPRED>                                */
    /************************************************************************/

    /// \brief Alias type for std::unordered_multimap with polymorphic allocator type.
    template <typename TKey, typename TValue, typename THash = std::hash<TKey>, typename TPred = std::equal_to<TKey>>
    using Multimap = std::unordered_multimap<TKey, TValue, THash, TPred, PolymorphicAllocator<std::pair<const TKey, TValue>>>;

    /************************************************************************/
    /* TRAITS                                                               */
    /************************************************************************/

    /// \brief If TType represents a map provides the members constant value equal to true, otherwise value is false.
    /// \author Raffaele D. Facendola - November 2016
    template <typename TType>
    struct is_map : std::false_type {};

    /// \brief Specialization for maps.
    template <typename TKey, typename TValue>
    struct is_map<Map<TKey, TValue>> : std::true_type {};

    /// \brief Specialization for multimaps.
    template <typename TKey, typename TValue>
    struct is_map<Multimap<TKey, TValue>> : std::true_type {};

    /// \brief Helper value for is_map<TType>.
    template <typename TType>
    constexpr bool is_map_v = is_map<TType>::value;

}
