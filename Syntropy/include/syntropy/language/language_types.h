
/// \file language_types.h
/// \brief This header is part of Syntropy language module. It contains definitions for fundamental data types.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace Syntropy
{
    /************************************************************************/
    /* FUNDAMENTAL TYPES                                                    */
    /************************************************************************/

    /// \brief Boolean value.
    /// \remarks Standard booleans bind also to numeric values.
    using Bool = bool;

    /// \brief 64-bit integer value.
    using Int = std::int64_t;

    /// \brief 32-bit floating point value.
    using Float = float;

    /************************************************************************/
    /* POINTER TYPES                                                        */
    /************************************************************************/

    /// \brief Type alias for a non-owning pointer which doesn't participate to pointee life-time.
    /// The pointee can either be read-only or read-write.
    template <typename TType>
    using PointerT = TType*;

    /// \brief Type alias for a non-owning pointer to a read-only pointee. This pointer doesn't participate to pointee life-time.
    /// This pointer binds also to RWPointers automatically.
    /// This type can be used to initialize pointers as: auto p = Pointer<Foo>{ nullptr };
    template <typename TType>
    using Pointer = PointerT<const TType>;

    /// \brief Type alias for a non-owning pointer to a read-write pointee. This pointer doesn't participate to pointee life-time.
    /// This type can be used to initialize pointers as: auto p = Pointer<Foo>{ nullptr };
    template <typename TType, typename = std::enable_if_t<!std::is_const_v<TType>>>
    using RWPointer = PointerT<TType>;

    /************************************************************************/
    /* SPECIAL TYPES                                                        */
    /************************************************************************/

    /// \brief Type of the null pointer literal (nullptr).
    using Null = std::nullptr_t;

    /************************************************************************/
    /* TYPE CAST                                                            */
    /************************************************************************/

    /// \brief Convert rhs to a boolean.
    template <typename TType>
    constexpr Bool ToBool(TType rhs) noexcept;

    /// \brief Truncate rhs to an integer value.
    template <typename TNumber>
    constexpr Int ToInt(TNumber rhs) noexcept;

    /// \brief Convert rhs to a floating point value.
    template <typename TNumber>
    constexpr Float ToFloat(TNumber rhs) noexcept;

    /// \brief Convert rhs to a pointer to UType.
    /// \remarks If the pointee type is not related to UType, the program is ill-formed.
    template <typename TType, typename UType>
    constexpr PointerT<TType> ToPointer(PointerT<UType> rhs) noexcept;

    /************************************************************************/
    /* READ ONLY \ READ WRITE                                               */
    /************************************************************************/

    /// \brief Convert rhs to a read-only value.
    template <typename TType>
    constexpr const TType& ReadOnly(const TType& rhs) noexcept;

    /// \brief R-value references shall not be converted to read-only: they are being thrown away \ moved.
    template <typename TType>
    constexpr void ReadOnly(const TType&& rhs) noexcept = delete;

    /// \brief Convert rhs to a read-write value.
    /// \remarks If rhs doesn't refer to a read-write value, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr TType& ReadWrite(const TType& rhs) noexcept;

    /// \brief Convert rhs to a pointer to a read-only value.
    template <typename TType>
    constexpr Pointer<TType> ReadOnly(Pointer<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a read-write value.
    /// \remarks If rhs pointee doesn't refer to a read-write value, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr RWPointer<TType> ReadWrite(Pointer<TType> rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Type cast.
    // ==========

    template <typename TType>
    constexpr Bool ToBool(TType rhs) noexcept
    {
        return !!rhs;
    }

    template <typename TNumber>
    constexpr Int ToInt(TNumber rhs) noexcept
    {
        return static_cast<Int>(rhs);
    }

    template <typename TNumber>
    constexpr Float ToFloat(TNumber rhs) noexcept
    {
        return static_cast<Float>(rhs);
    }

    template <typename TType, typename UType>
    constexpr PointerT<TType> ToPointer(PointerT<UType> rhs) noexcept
    {
        return static_cast<PointerT<TType>>(rhs);
    }

    // Read-Only \ Read-Write.
    // =======================

    template <typename TType>
    constexpr const TType& ReadOnly(const TType& rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr TType& ReadWrite(const TType& rhs) noexcept
    {
        return const_cast<TType&>(rhs);
    }

    template <typename TType>
    constexpr Pointer<TType> ReadOnly(Pointer<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr RWPointer<TType> ReadWrite(Pointer<TType> rhs) noexcept
    {
        return rhs ? ReadWrite(*rhs) : nullptr;
    }

}


