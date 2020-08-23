
/// \file support.h
/// \brief This header is part of Syntropy language module. It contains auxiliary definitions for fundamentals types.
///
/// \author Raffaele D. Facendola - August  2020

#pragma once

#include <type_traits>

#include "syntropy/language/fundamentals/types.h"
#include "syntropy/language/type_traits/manipulation.h"

namespace Syntropy
{
    /************************************************************************/
    /* MOVE                                                                 */
    /************************************************************************/

    /// \brief Indicate that rhs may be "moved from", allowing for efficient transfer of resources from rhs to another object.
    template <typename TType>
    [[nodiscard]] constexpr Traits::RemoveReference<TType>&& Move(TType&& rhs) noexcept;

    /************************************************************************/
    /* FORWARD                                                              */
    /************************************************************************/

    /// brief Forward an lvalue as either an lvalue or rvalue.
    template <typename TType>
    [[nodiscard]] constexpr TType&& Forward(Traits::RemoveReference<TType>& rhs) noexcept;

    /// \brief Forward an rvalue as an rvalue.
    template <typename TType>
    [[nodiscard]] constexpr TType&& Forward(Traits::RemoveReference<TType>&& rhs) noexcept;

    /************************************************************************/
    /* DECLVAL                                                              */
    /************************************************************************/

    /// \brief Convert TType as a reference type, without calling any constructor.
    /// \remarks this function shall never be evaluated as it has no definition.
    template <typename TType>
    Traits::AddRValueReference<TType> Declval() noexcept;

    /************************************************************************/
    /* SWAP                                                                 */
    /************************************************************************/

    // Swap both lhs and rhs.
    template <typename TType>
    constexpr void Swap(TType& lhs, TType& rhs) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Move.
    // =====

    template <typename TType>
    constexpr Traits::RemoveReference<TType>&& Move(TType&& rhs) noexcept
    {
        return static_cast<Traits::RemoveReference<TType>&&>(rhs);
    }

    // Forward.
    // ========

    template <typename TType>
    constexpr TType&& Forward(Traits::RemoveReference<TType>& rhs) noexcept
    {
        return static_cast<TType&&>(rhs);
    }

    template <typename TType>
    constexpr TType&& Forward(Traits::RemoveReference<TType>&& rhs) noexcept
    {
        return static_cast<TType&&>(rhs);
    }

    // Swap.
    // =====

    template <typename TType>
    constexpr void Swap(TType& lhs, TType& rhs) noexcept
    {
        auto xhs = Move(lhs);

        lhs = Move(rhs);
        rhs = Move(xhs);
    }

}


