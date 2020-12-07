
/// \file byte.h
/// \brief This header is part of Syntropy memory module. It contains definitions for byte definitions.
///
/// \author Raffaele D. Facendola - Aug 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FUNDAMENTAL TYPES                                                    */
    /************************************************************************/

    /// \brief Represents a single byte.
    /// This type is neither a character type, nor an arithmetic type.
    enum class Byte : std::int8_t {};

    /************************************************************************/
    /* POINTER TYPES                                                        */
    /************************************************************************/

    /// \brief A non-owning raw pointer to an immutable memory location.
    using ImmutableBytePtr = ImmutablePtr<Byte>;

    /// \brief A non-owning raw pointer to a mutable memory location.
    using MutableBytePtr = MutablePtr<Byte>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Byte pointer casts.
    // ===================

    /// \brief Convert rhs to a byte value.
    template <typename TNumber>
    constexpr Byte ToByte(TNumber rhs) noexcept;

    /// \brief Convert rhs to an immutable byte pointer.
    template <typename TType>
    constexpr ImmutableBytePtr ToImmutableBytePtr(ImmutablePtr<TType> rhs) noexcept;

    /// \brief Convert rhs to a mutable byte pointer.
    template <typename TType>
    constexpr MutableBytePtr ToMutableBytePtr(MutablePtr<TType> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Byte pointer casts.

    template <typename TNumber>
    constexpr Byte ToByte(TNumber rhs) noexcept
    {
        return static_cast<Byte>(rhs);
    }

    template <typename TType>
    constexpr ImmutableBytePtr ToImmutableBytePtr(ImmutablePtr<TType> rhs) noexcept
    {
        return reinterpret_cast<ImmutableBytePtr>(rhs);
    }

    template <typename TType>
    constexpr MutableBytePtr ToMutableBytePtr(MutablePtr<TType> rhs) noexcept
    {
        return reinterpret_cast<MutableBytePtr>(rhs);
    }
}

// ===========================================================================
