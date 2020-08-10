
/// \file combinatorics.h
/// \brief This header is part of the Syntropy math module. It contains classes and functions to handle combinatorial concepts.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <iterator>

#include "syntropy/core/range.h"
#include "syntropy/math/random_distributions.h"

namespace Syntropy
{
    /************************************************************************/
    /* RANDOM                                                               */
    /************************************************************************/

    /// \brief Exposes combinatorial utilities.
    /// \author Raffaele D. Facendola - 2020.
    namespace Random
    {
        /// \brief Swap a random element inside the range rhs with the first element in rhs.
        /// \returns Return a range which starts past the first element in rhs or an empty range if there's no such element.
        template <typename TIterator>
        Range<TIterator> Pick(const Range<TIterator>& rhs);

        /// \brief Randomly shuffle elements in the range [range.begin; middle) picking random elements from the provided range.
        /// After this call the range [range.begin; middle) contains shuffled elements, while the content of [middle; range.end) contains the remaining elements in unspecified order.
        /// \param range Range to shuffle.
        /// \param middle Iterator past the last element in the shuffled collection.
        template <typename TIterator>
        Range<TIterator> ShufflePartial(const Range<TIterator>& rhs, TIterator middle);

        /// \brief Randomly shuffle elements in rhs, in place.
        template <typename TIterator>
        void Shuffle(const Range<TIterator>& rhs);

    }

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Random.

    template <typename TIterator>
    inline Range<TIterator> Random::Pick(const Range<TIterator>& rhs)
    {
        auto range = rhs;

        if (range)
        {
            auto index = Uniform(range.GetSize());

            std::iter_swap(range.Begin(), range.Begin() + index);

            range.PopFront();
        }

        return range;
    }

    template <typename TIterator>
    inline Range<TIterator> Random::ShufflePartial(const Range<TIterator>& rhs, TIterator middle)
    {
        //Fisher-Yates shuffle.

        auto range = rhs;

        for (; range && (range.Begin() != middle); range = Pick(range));

        return range;
    }

    template <typename TIterator>
    inline void Random::Shuffle(const Range<TIterator>& rhs)
    {
        ShufflePartial(rhs, rhs.End());
    }

}