
/// \file references.h
/// \brief This header is part of Syntropy language module. It contains definitions for references.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include "syntropy/language/templates/traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* REFERENCE TYPES                                                      */
    /************************************************************************/

    /// \brief Type alias for a lvalue reference to a *read-only* object.
    template <typename TType>
    using Reference = const TType&;

    /// \brief Type alias for a lvalue reference to a *read-write* object.
    template <typename TType, typename = Templates::EnableIf<!Templates::IsConst<Templates::RemoveReference<TType&>>>>
    using RWReference = TType&;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Indicate that rhs may be "moved from", allowing for efficient transfer of resources from rhs to another object.
    template <typename TType>
    [[nodiscard]] constexpr Templates::RemoveReference<TType>&& Move(TType&& rhs) noexcept;

    /// brief Forward an lvalue as either a lvalue or rvalue and rvalues to rvalues. Forwarding a rvalue as a lvalue is forbidden.
    template <typename TType>
    [[nodiscard]] constexpr TType&& Forward(Templates::RemoveReference<TType>& rhs) noexcept;

    /// brief Forward an lvalue as either a lvalue or rvalue and rvalues to rvalues. Forwarding a rvalue as a lvalue is forbidden.
    template <typename TType>
    [[nodiscard]] constexpr TType&& Forward(Templates::RemoveReference<TType>&& rhs) noexcept;

    /// \brief Convert rhs to a read-only reference.
    template <typename TType>
    [[nodiscard]] constexpr Reference<TType> ReadOnly(Reference<TType> rhs) noexcept;

    /// \brief Rvalues shall not be converted to read-only.
    template <typename TType>
    [[nodiscard]] constexpr void ReadOnly(const TType&& rhs) noexcept = delete;

    /// \brief Convert rhs to a read-write reference.
    /// The intended use for this method is to write a non-const implementation based on a const implementation, without duplicating code.
    /// Such usage has the form: ReadWrite(F(ReadOnly(x))) where x is non-const and F(.) is a function.
    /// \remarks If rhs doesn't refer to a read-write object, accessing the result of this method results in undefined behavior.
    template <typename TType>
    [[nodiscard]] constexpr RWReference<TType> ReadWrite(Reference<TType> rhs) noexcept;

    /// \brief Rvalues shall not be converted to read-write.
    template <typename TType>
    [[nodiscard]] constexpr void ReadWrite(const TType&& rhs) noexcept = delete;
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    template <typename TType>
    constexpr Templates::RemoveReference<TType>&& Move(TType&& rhs) noexcept
    {
        return static_cast<Templates::RemoveReference<TType>&&>(rhs);
    }

    template <typename TType>
    constexpr TType&& Forward(Templates::RemoveReference<TType>& rhs) noexcept
    {
        return static_cast<TType&&>(rhs);
    }

    template <typename TType>
    constexpr TType&& Forward(Templates::RemoveReference<TType>&& rhs) noexcept
    {
        return static_cast<TType&&>(rhs);
    }

    template <typename TType>
    constexpr Reference<TType> ReadOnly(Reference<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr RWReference<TType> ReadWrite(Reference<TType> rhs) noexcept
    {
        return const_cast<TType&>(rhs);
    }

}

// ===========================================================================
