
/// \file references.h
/// \brief This header is part of Syntropy language module. It contains definitions for reference types.
/// 
/// \author Raffaele D. Facendola - Dic 2020

#pragma once

#include <cstdint>
#include <cstddef>

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
    template <typename TType>
    using Immovable = const TType&&;

    /// \brief RValue reference to an mutable instance of type TType.
    template <typename TType>
    using Movable = TType&&;

    /************************************************************************/
    /* FUNCTIONS                                                            */
    /************************************************************************/

    // Access.
    // =======

    /// \brief Convert rhs to a const reference.
    template <typename TType>
    constexpr Immutable<TType> ToConst(Immutable<TType> rhs) noexcept;

    /// \brief Convert rhs to a const value.
    /// \remarks This method move-constructs a new constant value since rvalue references may cease to exists when the method returns.
    template <typename TType>
    constexpr Val<TType> ToConst(Movable<TType> rhs) noexcept;

    /// \brief Convert rhs to a read-write reference.
    /// The intended use for this method is to write a non-const implementation based on a const implementation, without duplicating code.
    /// Such usage has the form: ReadWrite(F(ReadOnly(x))) where x is non-const and F(.) is a function.
    /// \remarks If rhs doesn't refer to a read-write object, accessing the result of this method results in undefined behavior.
    template <typename TType>
    constexpr Mutable<TType> ToMutable(Immutable<TType> rhs) noexcept;

    /// \brief Convert rhs to a mutable value.
    /// \remarks This method move-constructs a new constant value since rvalue references may cease to exists when the method returns.
    template <typename TType>
    constexpr TType ToMutable(Movable<TType> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Access.
    // =======

    template <typename TType>
    constexpr Immutable<TType> ToConst(Immutable<TType> rhs) noexcept
    {
        return rhs;
    }

    template <typename TType>
    constexpr Val<TType> ToConst(Movable<TType> rhs) noexcept
    {
        return Val<TType>{ Move(rhs) };
    }

    template <typename TType>
    constexpr Mutable<TType> ToMutable(Immutable<TType> rhs) noexcept
    {
        return const_cast<Mutable<TType>>(rhs);
    }

    template <typename TType>
    constexpr TType ToMutable(Movable<TType> rhs) noexcept
    {
        return TType{ Move(rhs) };
    }

}

// ===========================================================================
