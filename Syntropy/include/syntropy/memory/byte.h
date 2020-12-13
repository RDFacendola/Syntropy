
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
    /* BYTE TYPES                                                           */
    /************************************************************************/

    /// \brief Represents a single byte.
    /// This type is neither a character type, nor an arithmetic type.
    enum class Byte : std::int8_t {};

    /************************************************************************/
    /* BYTE POINTER TYPES                                                   */
    /************************************************************************/

    /// \brief A non-owning raw pointer to a read-only memory location.
    using BytePtr = Ptr<Byte>;

    /// \brief A non-owning raw pointer to a read-write memory location.
    using RWBytePtr = RWPtr<Byte>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Conversion.
    // ===========

    /// \brief Convert rhs to a byte value, truncating the result if needed.
    template <typename TNumber>
    constexpr Byte ToByte(TNumber rhs) noexcept;

    /// \brief Convert rhs to a pointer to a read-only memory location.
    template <typename TType>
    BytePtr ToBytePtr(Immutable<BasePtr<TType>> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a read-write memory location.
    /// \remarks If rhs doesn't refer to a mutable memory location, accessing the returned value results in undefined behavior.
    template <typename TType>
    RWBytePtr ToBytePtr(Immutable<BasePtr<TType>> rhs) noexcept;

    /// \brief Convert a raw-pointer to a strongly-typed read-only instance of TType.
    template <typename TType>
    Ptr<TType> FromBytePtr(Immutable<BytePtr> rhs) noexcept;

    /// \brief Convert a raw-pointer to a strongly-typed read-write instance of TType.
    /// \remarks If the raw-pointer doesn't refer to a read-writable memory location, accessing the returned value results in undefined behavior.
    template <typename TType>
    RWPtr<TType> FromBytePtr(Immutable<RWBytePtr> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    template <typename TNumber>
    constexpr Byte ToByte(TNumber rhs) noexcept
    {
        return static_cast<Byte>(rhs);
    }

    template <typename TType>
    inline BytePtr ToBytePtr(Immutable<BasePtr<TType>> rhs) noexcept
    {
        return reinterpret_cast<BytePtr>(rhs);
    }

    template <typename TType>
    inline RWBytePtr ToBytePtr(Immutable<BasePtr<TType>> rhs) noexcept
    {
        return const_cast<RWBytePtr>(reinterpret_cast<BytePtr>(rhs));
    }

    template <typename TType>
    inline Ptr<TType> FromBytePtr(Immutable<BytePtr> rhs) noexcept
    {
        return reinterpret_cast<Ptr<TType>>(rhs);
    }

    template <typename TType>
    inline RWPtr<TType> FromBytePtr(Immutable<RWBytePtr> rhs) noexcept
    {
        return const_cast<RWPtr<TType>>(reinterpret_cast<Ptr<TType>>(rhs));
    }
}

// ===========================================================================
