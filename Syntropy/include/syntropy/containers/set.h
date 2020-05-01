
/// \file set.h
/// \brief This header is part of the Syntropy container module. It contains wrappers for sets and multisets.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <unordered_set>
#include <functional>

#include "syntropy/language/utility.h"

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

}
