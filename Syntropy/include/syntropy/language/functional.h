
/// \file functional.h
/// \brief This header is part of Syntropy language module. It contains extensions to standard header <functional>
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/core/types.h"
#include "syntropy/syntropy.h"

#include <functional>

namespace syntropy
{
    /************************************************************************/
    /* HASH FUNCTOR 64                                                      */
    /************************************************************************/

    /// \brief Functor object used to compute non-cryptographic 64-bit hash.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TType>
    struct HashFunctor64
    {
        /// \brief Get the non-cryptographic 64-bit hash of rhs.
        Int operator()(const TType& rhs) const;
    };

    /************************************************************************/
    /* HASH FUNCTOR 32                                                      */
    /************************************************************************/

    /// \brief Functor object used to compute non-cryptographic 32-bit hash.
    /// \author Raffaele D. Facendola - May 2020.
    template <typename TType>
    struct HashFunctor32
    {
        /// \brief Get the non-cryptographic 32-bit hash of rhs.
        Int operator()(const TType& rhs) const;
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Get the non-cryptographic 64-bit hash of an instance.
    template <typename TType>
    Int Hash64(const TType& rhs);

    /// \brief Get the non-cryptographic 32-bit hash of an instance.
    template <typename TType>
    Int Hash32(const TType& rhs);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // HashFunctor64<TType>.

    template <typename TType>
    inline Int HashFunctor64<TType>::operator()(const TType& rhs) const
    {
        return Hash64(rhs);
    }

    // HashFunctor32<TType>.

    template <typename TType>
    inline Int HashFunctor32<TType>::operator()(const TType& rhs) const
    {
        return Hash32(rhs);
    }

    // Non-member functions.

    template <typename TType>
    inline Int Hash64(const TType& rhs)
    {
        return static_cast<Int>(std::hash<TType>{}(rhs));
    }

    template <typename TType>
    inline Int Hash32(const TType& rhs)
    {
        using syntropy::Hash64;

        return Hash64(rhs) & 0xFFFFFFFF;
    }

}