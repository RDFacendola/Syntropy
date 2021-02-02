
/// \file references.h
///
/// \brief This header is part of Syntropy language module.
///        It contains definitions for reference types.
///
/// \author Raffaele D. Facendola - December 2020

#pragma once

#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* REFERENCE TYPES                                                      */
    /************************************************************************/

    /// \brief Lvalue reference to an immutable instance of type TType.
    template <typename TType>
    using Immutable = const TType&;

    /// \brief LValue reference to a mutable instance of type TType.
    template <typename TType>
    using Mutable = TType&;

    /// \brief RValue reference to an immutable instance of type TType.
    ///
    /// This type binds to rvalues and rvalues only as it is never
    /// interpreted as a forwarding reference, making it the ideal tool
    /// to delete functions overloads (see: AddressOf()).
    template <typename TType>
    using Immovable = const TType&&;

    /// \brief RValue reference to an mutable instance of type TType.
    template <typename TType>
    using Movable = TType&&;

    /************************************************************************/
    /* TEMPLATES                                                            */
    /************************************************************************/

    /// \brief Lvalue reference to either a mutable or immutable
    ///        instance of type TType.
    ///
    /// The difference between this type and Mutable<T> is purely semantic.
    template <typename TType>
    using Reference = TType&;

    /// \brief A forwarding reference that propagates rvalues
    ///        as rvalues and lvalues as either lvalues or rvalues.
    ///
    /// \remarks The difference between this type and Movable<T>
    ///          is purely semantic.
    template <typename TType>
    using Forwarding = TType&&;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Copy.
    // =====

    /// \brief Perform a copy of an instance by means of copy constructor.
    template <typename TType>
    constexpr TType
    Copy(Immutable<TType> rhs) noexcept;

    // Move.
    // =====

    /// \brief Indicate that rhs may be "moved from", allowing for efficient
    ///        transfer of resources from rhs to another object.
    template <typename TType>
    constexpr Forwarding<Templates::RemoveReference<TType>>
    Move(Forwarding<TType> rhs) noexcept;

    // Perfect forwarding.
    // ===================

    /// \brief Forward an lvalue as either a lvalue or rvalue and rvalues
    ///        to rvalues. Forwarding a rvalue as a lvalue is forbidden.
    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Mutable<Templates::RemoveReference<TType>> rhs) noexcept;

    /// brief Forward an lvalue as either a lvalue or rvalue and rvalues
    ///        to rvalues. Forwarding a rvalue as a lvalue is forbidden.
    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Movable<Templates::RemoveReference<TType>> rhs) noexcept;

    // Access.
    // =======

    /// \brief Convert rhs to a reference to an immutable
    ///        instance of type TType.
    template <typename TType>
    constexpr Immutable<TType>
    ToImmutable(Immutable<TType> rhs) noexcept;

    /// \brief Deleted overload to disallow rvalue arguments.
    template <typename TType>
    constexpr void
    ToImmutable(Immovable<TType> rhs) noexcept = delete;

    /// \brief Convert rhs to a mutable reference to an instance of type TType.
    ///
    /// The intended use for this method is to write a non-const implementation
    /// based on a const implementation, without duplicating code.
    /// Such usage has the form: ToMutable(F(ToImmutable(x)))
    /// where x is non-const and F(.) is a function.
    ///
    /// \remarks If rhs doesn't refer to a mutable object, accessing
    ///          the result of this method results in undefined behavior.
    template <typename TType>
    constexpr Mutable<Templates::RemoveConst<TType>>
    ToMutable(Immutable<TType> rhs) noexcept;

    /// \brief Deleted overload to disallow rvalue arguments.
    template <typename TType>
    constexpr TType
    ToMutable(Immovable<TType> rhs) noexcept = delete;

    /// \brief Convert rhs to a reference to an immovable instance
    ///        of type TType.
    template <typename TType>
    constexpr Immovable<TType>
    ToImmovable(Immutable<TType> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Non-member functions.
    // =====================

    // Copy.

    template <typename TType>
    constexpr TType
    Copy(Immutable<TType> rhs) noexcept
    {
        return rhs;
    }

    // Move.

    template <typename TType>
    constexpr Forwarding<Templates::RemoveReference<TType>>
    Move(Forwarding<TType> rhs) noexcept
    {
        return static_cast<Movable<Templates::RemoveReference<TType>>>(rhs);
    }

    // Perfect forwarding.

    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Templates::RemoveReference<TType>& rhs) noexcept
    {
        return static_cast<Forwarding<TType>>(rhs);
    }

    template <typename TType>
    constexpr Forwarding<TType>
    Forward(Templates::RemoveReference<TType>&& rhs) noexcept
    {
        return static_cast<Forwarding<TType>>(rhs);
    }

    // Access.

    template <typename TType>
    constexpr Immutable<TType>
    ToImmutable(Immutable<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr Mutable<Templates::RemoveConst<TType>>
    ToMutable(Immutable<TType> rhs) noexcept
    {
        return const_cast<Mutable<TType>>(rhs);
    }

    template <typename TType>
    constexpr Immovable<TType>
    ToImmovable(Immutable<TType> rhs) noexcept
    {
        return static_cast<Immovable<TType>>(rhs);
    }

}

// ===========================================================================
