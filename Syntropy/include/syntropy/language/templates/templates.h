
/// \file templates.h
/// \brief This header is part of the Syntropy language module. It contains fundamental template definitions.
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
    /* FORWARD                                                              */
    /************************************************************************/

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

    // Forward.
    // ========

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
