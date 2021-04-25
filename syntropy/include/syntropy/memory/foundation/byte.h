
/// \file byte.h
///
/// \brief This header is part of Syntropy memory module.
///        It contains definitions for byte definitions.
///
/// \author Raffaele D. Facendola - August 2020.

// ===========================================================================

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
    [[nodiscard]] constexpr Byte
    ToByte(TNumber rhs) noexcept;

    /// \brief Convert rhs to a pointer to a read-only memory location.
    template <typename TType>
    [[nodiscard]] BytePtr
    ToBytePtr(Ptr<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a read-write memory location.
    /// \remarks If rhs doesn't refer to a mutable memory location, accessing
    ///          the returned value results in undefined behavior.
    template <typename TType>
    [[nodiscard]] RWBytePtr
    ToBytePtr(RWPtr<TType> rhs) noexcept;

    /// \brief Convert a raw-pointer to a strongly-typed read-only
    ///        instance of TType.
    template <typename TType>
    [[nodiscard]] Ptr<TType>
    FromBytePtr(BytePtr rhs) noexcept;

    /// \brief Convert a raw-pointer to a strongly-typed read-write instance
    ///        of TType.
    /// \remarks If the raw-pointer doesn't refer to a read-writable memory
    ///          location, accessing the returned value results in undefined
    ///          behavior.
    template <typename TType>
    [[nodiscard]] RWPtr<TType>
    FromBytePtr(RWBytePtr rhs) noexcept;

}

// ===========================================================================

#include "details/byte.inl"

// ===========================================================================
