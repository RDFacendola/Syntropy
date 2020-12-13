
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
    /// \remarks The pointee is expected to be a generic memory location: see TypelessPtr for object instances.
    using ImmutableBytePtr = Ptr<Byte>;

    /// \brief A non-owning raw pointer to a mutable memory location.
    /// \remarks The pointee is expected to be a generic memory location: see RWTypelessPtr for object instances.
    using MutableBytePtr = RWPtr<Byte>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Conversions.
    // ============

    /// \brief Convert rhs to a byte value.
    template <typename TNumber>
    constexpr Byte ToByte(TNumber rhs) noexcept;

    /// \brief Convert rhs to a pointer to an immutable memory location.
    ImmutableBytePtr ToBytePtr(TypelessPtr rhs) noexcept;

    /// \brief Convert rhs to a pointer to a mutable memory location.
    /// \remarks If rhs doesn't refer to a mutable memory location, accessing the returned value results in undefined behavior.
    MutableBytePtr ToBytePtr(RWTypelessPtr rhs) noexcept;

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

    inline ImmutableBytePtr ToBytePtr(TypelessPtr rhs) noexcept
    {
        return reinterpret_cast<ImmutableBytePtr>(rhs);
    }

    inline MutableBytePtr ToBytePtr(RWTypelessPtr rhs) noexcept
    {
        return reinterpret_cast<MutableBytePtr>(rhs);
    }
}

// ===========================================================================
