
/// \file record.details.h
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/templates/templates.h"

#include "syntropy/core/records/record_extensions.h"

// ===========================================================================

namespace Syntropy::Records::Details
{
    /************************************************************************/
    /* CUSTOMIZATION-POINT-OBJECTS                                          */
    /************************************************************************/

    // Core idea based on this amazing post:
    // https://wandbox.org/permlink/AB9uQxO2MymNDDtt

    /// \brief Priority of a custom extension implementation.
    using ExtensionPriority = Templates::Priority<4>;

    /// \brief Priority of a member-function implementation.
    using MemberFunctionPriority = Templates::Priority<3>;

    /// \brief Priority of a member-operator implementation.
    using MemberOperatorPriority = Templates::Priority<2>;

    /// \brief Priority of a non-member-function implementation.
    using NonMemberFunctionPriority = Templates::Priority<1>;

    /// \brief Priority of a fallback implementation.
    using FallbackPriority = Templates::Priority<0>;

    /// \brief Highest priority among method implementations.
    inline constexpr Templates::Priority kMaxPriority = ExtensionPriority{};

    /************************************************************************/
    /* GET                                                                  */
    /************************************************************************/

    /// \brief Custom extension.
    template <Int TIndex, typename TRecord>
    inline auto
    InvokeGet(Forwarding<TRecord> sequence, ExtensionPriority)
        noexcept -> decltype(Extensions::Get<TIndex, TRecord>{}(
            Forward<TRecord>(sequence)));

    /// \brief Member-function.
    template <Int TIndex, typename TRecord>
    inline auto
    InvokeGet(Forwarding<TRecord> sequence, MemberFunctionPriority)
        noexcept -> decltype(sequence.template Get<TIndex>());

    /// \brief Non-member function, possibly using ADL.
    template <Int TIndex, typename TRecord>
    inline auto
    InvokeGet(Forwarding<TRecord> sequence, NonMemberFunctionPriority)
        noexcept -> decltype(Get<TIndex>(Forward<TRecord>(sequence)));

    /// \brief Routes the invocation.
    template <Int TIndex, typename TRecord>
    inline auto
    RouteGet(Forwarding<TRecord> sequence)
        noexcept -> decltype(InvokeGet<TIndex>(Forward<TRecord>(sequence),
                                               kMaxPriority));
}

// ===========================================================================
