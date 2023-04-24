
/// \file view.h
///
/// \brief This header is part of Syntropy language module.
///        It contains view-related definitions.
///
/// \author Raffaele D. Facendola - May 2021.

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/concepts.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* SUPPORT EXTENSIONS                                                   */
    /************************************************************************/

    namespace Support::Extensions
    {
        /// \brief Get a read-only view to an object.
        template <typename TType, typename = void>
        struct ToReadOnly;

        /// \brief Get a read-write view to an object.
        template <typename TType, typename = void>
        struct ToReadWrite;
    }

    /************************************************************************/
    /* SUPPORT                                                              */
    /************************************************************************/

    namespace Support
    {
        /// \brief Convert rhs to a read-only view.
        template <typename TType>
        [[nodiscard]] constexpr auto
        ToReadOnly(Forwarding<TType> rhs) noexcept;

        /// \brief Convert rhs to a read-write view.
        ///
        /// The intended use for this method is to write a read-write
        /// implementation based on a read-only one, without duplicating code.
        /// Such usage has the form: ToReadWrite(F(ToReadOnly(x)))
        /// where x is mutable and F(.) is a function.
        ///
        /// \remarks If rhs doesn't refer to a read write value, accessing the
        ///          returned value results in undefined behavior.
        template <typename TType>
        [[nodiscard]] constexpr auto
        ToReadWrite(Forwarding<TType> rhs) noexcept;
    }

}

// ===========================================================================

#include "details/view.inl"

// ===========================================================================
