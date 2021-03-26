
/// \file record.details.inl
///
/// \author Raffaele D. Facendola - March 2021

#pragma once

// ===========================================================================

namespace Syntropy::Records::Details
{
    /************************************************************************/
    /* GET                                                                  */
    /************************************************************************/

    template <Int TIndex, typename TRecord>
    inline auto
    InvokeGet(Forwarding<TRecord> sequence, ExtensionPriority)
        noexcept -> decltype(Extensions::Get<TIndex, TRecord>{}(
            Forward<TRecord>(sequence)))
    {
        return Extensions::Get<TIndex, TRecord>{}(Forward<TRecord>(sequence));
    }

    template <Int TIndex, typename TRecord>
    inline auto
    InvokeGet(Forwarding<TRecord> sequence, MemberFunctionPriority)
        noexcept -> decltype(sequence.template Get<TIndex>())
    {
        return sequence.template Get<TIndex>();
    }

    template <Int TIndex, typename TRecord>
    inline auto
    InvokeGet(Forwarding<TRecord> sequence, NonMemberFunctionPriority)
        noexcept -> decltype(Get<TIndex>(Forward<TRecord>(sequence)))
    {
        return Get<TIndex>(Forward<TRecord>(sequence));
    }

    template <Int TIndex, typename TRecord>
    inline auto
    RouteGet(Forwarding<TRecord> sequence)
        noexcept -> decltype(InvokeGet<TIndex>(Forward<TRecord>(sequence),
                                               kMaxPriority))
    {
        return InvokeGet<TIndex>(Forward<TRecord>(sequence), kMaxPriority);
    }

}

// ===========================================================================
