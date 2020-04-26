
/// \file algorithm.h
/// \brief This file contains definitions of general algorithms.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <utility>

#include "syntropy/patterns/algorithm.h"

namespace syntropy
{
    /// \brief Check the range [first, last) for a token delimited by token_begin and token_end.
    /// \param first Iterator to the begin of the search range.
    /// \param last Iterator past the end of the search range.
    /// \param token_begin Initial delimiter of the token.
    /// \param token-end Final delimiter of the token.
    /// \return Returns a pair of iterators inside the search range representing the token found, if any.
    template <typename TIterator, typename TToken>
    std::pair<TIterator, TIterator> GetToken(TIterator first, TIterator last, const TToken& token_begin, const TToken& token_end)
    {
        auto token_first = last;

        for (; first != last; ++first)
        {
            if (*first == token_begin)
            {
                // Potential start of a new token
                token_first = first;
            }
            else if (*first == token_end && token_first != last)
            {
                // Token found
                return std::make_pair(token_first, ++first);
            }
        }

        // No token found
        return std::make_pair(last, last);
    }

}