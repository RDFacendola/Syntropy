
/// \file fundamentals.h
/// \brief This header is part of Syntropy language module. It contains fundamental definitions.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include <type_traits>

#include "syntropy/language/fundamentals/types.h"
#include "syntropy/language/fundamentals/casts.h"
#include "syntropy/language/fundamentals/support.h"

// Pointer (ReadOnly \ ReadWrite)
// Reference (ReadOnly \ ReadWrite)
// ReadOnly \ ReadWrite

namespace Syntropy
{
    /************************************************************************/
    /* POINTER TYPES                                                        */
    /************************************************************************/

    /// \brief Type alias for a non-owning pointer to a read-only pointee. This pointer doesn't participate to pointee life-time.
    /// This pointer binds also to RWPointers automatically.
    /// This type can be used to initialize pointers as: auto p = Pointer<Foo>{ nullptr };
    template <typename TType>
    using Pointer = PointerT<const TType>;

    /// \brief Type alias for a non-owning pointer to a read-write pointee. This pointer doesn't participate to pointee life-time.
    /// This type can be used to initialize pointers as: auto p = Pointer<Foo>{ nullptr };
    /// This type only binds to read-write objects.
    template <typename TType, typename = std::enable_if_t<!std::is_const_v<TType>>>
    using RWPointer = PointerT<TType>;

    /************************************************************************/
    /* REFERENCE TYPES                                                      */
    /************************************************************************/

    /// \brief Type alias for a l-value reference to a read-only object.
    /// This type binds also to RWReferences automatically.
    template <typename TType>
    using Reference = ReferenceT<const TType>;

    /// \brief Type alias for a l-value reference to a read-write object.
    /// This type only binds to read-write objects.
    template <typename TType, typename = std::enable_if_t<!std::is_const_v<TType>>>
    using RWReference = ReferenceT<TType>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Access modifiers.

    /// \brief Convert rhs to a pointer to a read-only value.
    template <typename TType>
    constexpr Pointer<TType> ReadOnly(Pointer<TType> rhs) noexcept;

    /// \brief Convert rhs to a pointer to a read-write value.
    /// \remarks If rhs pointee doesn't refer to a read-write value, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr RWPointer<TType> ReadWrite(Pointer<TType> rhs) noexcept;

    /// \brief Convert rhs to a read-only reference.
    template <typename TType>
    constexpr Reference<TType> ReadOnly(Reference<TType> rhs) noexcept;

    /// \brief R-value references shall not be converted to read-only: they are being thrown away \ moved.
    template <typename TType>
    constexpr void ReadOnly(const TType&& rhs) noexcept = delete;

    /// \brief Convert rhs to a read-write reference.
    /// \remarks If rhs doesn't refer to a read-write reference, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr RWReference<TType> ReadWrite(Reference<TType> rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Access modifiers.

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

    template <typename TType>
    constexpr Reference<TType> ReadOnly(Reference<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr RWReference<TType> ReadWrite(Reference<TType> rhs) noexcept
    {
        return const_cast<RWReference<TType>>(rhs);
    }

}


