
/// \file byte.h
/// \brief This header is part of Syntropy memory module. It contains definitions for memory-related fundamental data types.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include <ostream>

#include "syntropy/language/traits.h"
#include "syntropy/language/foundation.h"

namespace Syntropy
{
    /************************************************************************/
    /* FUNDAMENTAL TYPES                                                    */
    /************************************************************************/

    /// \brief Represents a single byte.
    /// This type is neither a character type, nor an arithmetic type.
    using Byte = std::byte;

    /************************************************************************/
    /* POINTER TYPES                                                        */
    /************************************************************************/

    /// \brief A non-owning raw pointer to a read-only memory location.
    using BytePtr = Pointer<Byte>;

    /// \brief A non-owning raw pointer to a read-write memory location.
    using RWBytePtr = RWPointer<Byte>;

    /// \brief A non-owning pointer to a typeless read-only memory location.
    /// A typeless pointer binds to any strongly-typed pointer.
    using TypelessPtr = Pointer<void>;

    /// \brief A non-owning pointer to a typeless read-write memory location.
    /// A read-write typeless pointer binds to any non-const strongly-typed pointer.
    using RWTypelessPtr = RWPointer<void>;

    /************************************************************************/
    /* TYPE CAST                                                            */
    /************************************************************************/

    /// \brief Convert a value to a byte value.
    template <typename TNumber>
    constexpr Byte ToByte(TNumber rhs) noexcept;

    /// \brief Convert rhs to a read-only byte pointer.
    template <typename TType>
    constexpr BytePtr ToBytePtr(Pointer<TType> rhs) noexcept;

    /// \brief Convert rhs to a read-write byte pointer.
    template <typename TType>
    constexpr RWBytePtr ToRWBytePtr(RWPointer<TType> rhs) noexcept;

    /// \brief Convert rhs to a strongly-typed read-only pointer type.
    /// \remarks If the pointee type is not related to TType, accessing the result of this method results in undefined behavior.
    template <typename TType>
    Pointer<TType> FromTypeless(TypelessPtr rhs) noexcept;

    /// \brief Convert rhs to a strongly-typed read-write pointer type.
    /// \remarks If the pointee type is not related to TType, accessing the result of this method results in undefined behavior.
    template <typename TType>
    RWPointer<TType> FromTypeless(RWTypelessPtr rhs) noexcept;

    /************************************************************************/
    /* STREAM INSERTION                                                     */
    /************************************************************************/

    /// \brief Stream insertion for byte types.
    std::ostream& operator<<(std::ostream& out, Byte rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Type cast.
    // ==========

    template <typename TNumber>
    constexpr Byte ToByte(TNumber rhs) noexcept
    {
        return static_cast<Byte>(rhs);
    }

    template <typename TType>
    constexpr BytePtr ToBytePtr(Pointer<TType> rhs) noexcept
    {
        return reinterpret_cast<BytePtr>(rhs);
    }

    template <typename TType>
    constexpr RWBytePtr ToRWBytePtr(RWPointer<TType> rhs) noexcept
    {
        return reinterpret_cast<RWBytePtr>(rhs);
    }

    template <typename TType>
    Pointer<TType> FromTypeless(TypelessPtr rhs) noexcept
    {
        return reinterpret_cast<Pointer<const TType>>(rhs);
    }

    template <typename TType>
    RWPointer<TType> FromTypeless(RWTypelessPtr rhs) noexcept
    {
        return reinterpret_cast<RWPointer<TType>>(rhs);
    }

    // Stream insertion.
    // =================

    inline std::ostream& operator<<(std::ostream& out, Byte rhs) noexcept
    {
        out << ToInt(rhs);

        return out;
    }

}


