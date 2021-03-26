
/// \file sequence.details.inl
///
/// \author Raffaele D. Facendola - March 2021

#pragma once

// ===========================================================================

namespace Syntropy::Sequences::Details
{
    /************************************************************************/
    /* GET                                                                  */
    /************************************************************************/

    template <Int TIndex, typename TSequence>
    inline auto
    InvokeGet(Forwarding<TSequence> sequence, ExtensionPriority)
        noexcept -> decltype(Extensions::Get<TIndex, TSequence>{}(
            Forward<TSequence>(sequence)))
    {
        return Extensions::Get<TIndex, TSequence>{}(Forward<TSequence>(sequence));
    }

    template <Int TIndex, typename TSequence>
    inline auto
    InvokeGet(Forwarding<TSequence> sequence, MemberFunctionPriority)
        noexcept -> decltype(sequence.template Get<TIndex>())
    {
        return sequence.template Get<TIndex>();
    }

    template <Int TIndex, typename TSequence>
    inline auto
    InvokeGet(Forwarding<TSequence> sequence, NonMemberFunctionPriority)
        noexcept -> decltype(Get<TIndex>(Forward<TSequence>(sequence)))
    {
        return Get<TIndex>(Forward<TSequence>(sequence));
    }

    template <Int TIndex, typename TSequence>
    inline auto
    RouteGet(Forwarding<TSequence> sequence)
        noexcept -> decltype(InvokeGet<TIndex>(Forward<TSequence>(sequence),
                                               kMaxPriority))
    {
        return InvokeGet<TIndex>(Forward<TSequence>(sequence), kMaxPriority);
    }

}

// ===========================================================================
