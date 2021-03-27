
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
    InvokeGet(Forwarding<TRecord> record, ExtensionPriority)
        noexcept -> decltype(Extensions::Get<TIndex, TRecord>{}(
            Forward<TRecord>(record)))
    {
        return Extensions::Get<TIndex, TRecord>{}(Forward<TRecord>(record));
    }

    template <Int TIndex, typename TRecord>
    inline auto
    InvokeGet(Forwarding<TRecord> record, MemberFunctionPriority)
        noexcept -> decltype(record.template Get<TIndex>())
    {
        return record.template Get<TIndex>();
    }

    template <Int TIndex, typename TRecord>
    inline auto
    InvokeGet(Forwarding<TRecord> record, NonMemberFunctionPriority)
        noexcept -> decltype(Get<TIndex>(Forward<TRecord>(record)))
    {
        return Get<TIndex>(Forward<TRecord>(record));
    }

    template <Int TIndex, typename TRecord>
    inline auto
    RouteGet(Forwarding<TRecord> record)
        noexcept -> decltype(InvokeGet<TIndex>(Forward<TRecord>(record),
                                               kMaxPriority))
    {
        return InvokeGet<TIndex>(Forward<TRecord>(record), kMaxPriority);
    }

}

// ===========================================================================
