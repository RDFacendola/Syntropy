
/// \file functional.h
/// \brief This header is part of the Syntropy language module. It contains definitions for fundamental functions.
///
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/templates/type_traits.h"

#include "syntropy/language/foundation/types.h"
#include "syntropy/language/foundation/pointers.h"
#include "syntropy/language/foundation/references.h"

// ===========================================================================

namespace Syntropy
{
        /************************************************************************/
    /* MOVE AND FORWARD                                                     */
    /************************************************************************/

    /// \brief Indicate that rhs may be "moved from", allowing for efficient transfer of resources from rhs to another object.
    template <typename TType>
    constexpr Templates::RemoveReference<TType>&& Move(TType&& rhs) noexcept;

    /// brief Forward an lvalue as either a lvalue or rvalue and rvalues to rvalues. Forwarding a rvalue as a lvalue is forbidden.
    template <typename TType>
    constexpr ForwardRef<TType> Forward(Templates::RemoveReference<TType>& rhs) noexcept;

    /// brief Forward an lvalue as either a lvalue or rvalue and rvalues to rvalues. Forwarding a rvalue as a lvalue is forbidden.
    template <typename TType>
    constexpr ForwardRef<TType> Forward(Templates::RemoveReference<TType>&& rhs) noexcept;

}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Access.
    // =======



    // Move and forward.
    // =================

    template <typename TType>
    constexpr Templates::RemoveReference<TType>&& Move(TType&& rhs) noexcept
    {
        return static_cast<Templates::RemoveReference<TType>&&>(rhs);
    }

    template <typename TType>
    constexpr ForwardRef<TType> Forward(Templates::RemoveReference<TType>& rhs) noexcept
    {
        return static_cast<ForwardRef<TType>>(rhs);
    }

    template <typename TType>
    constexpr ForwardRef<TType> Forward(Templates::RemoveReference<TType>&& rhs) noexcept
    {
        return static_cast<ForwardRef<TType>>(rhs);
    }

}

// ===========================================================================
