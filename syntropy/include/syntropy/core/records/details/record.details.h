
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
    InvokeGet(Forwarding<TRecord> record, ExtensionPriority)
        noexcept -> decltype(Extensions::Get<TIndex, TRecord>{}(
            Forward<TRecord>(record)));

    /// \brief Member-function.
    template <Int TIndex, typename TRecord>
    inline auto
    InvokeGet(Forwarding<TRecord> record, MemberFunctionPriority)
        noexcept -> decltype(record.template Get<TIndex>());

    /// \brief Non-member function, possibly using ADL.
    template <Int TIndex, typename TRecord>
    inline auto
    InvokeGet(Forwarding<TRecord> record, NonMemberFunctionPriority)
        noexcept -> decltype(Get<TIndex>(Forward<TRecord>(record)));

    /// \brief Routes the invocation.
    template <Int TIndex, typename TRecord>
    inline auto
    RouteGet(Forwarding<TRecord> record)
        noexcept -> decltype(InvokeGet<TIndex>(Forward<TRecord>(record),
                                               kMaxPriority));

    /************************************************************************/
    /* TYPE TRAITS                                                          */
    /************************************************************************/

    /// \brief Number of elements in a record.
    template <typename TRecord,
              template <typename> typename TRankTrait>
    inline constexpr Int
    RankOf = TRankTrait<Templates::UnqualifiedOf<TRecord>>::kValue;

    /// \brief True if two records have the same rank, false otherwise.
    template <typename TRecord,
              typename URecord,
              template <typename> typename TRankTrait>
    inline constexpr Bool
    IsSameRank =
        (RankOf<TRecord, TRankTrait> == RankOf<URecord, TRankTrait>);

    /// \brief Type of a record element.
    template <Int TIndex,
              typename TRecord,
              template <Int, typename> typename TElementTypeTrait>
    using
    ElementTypeOf = typename TElementTypeTrait<
        TIndex, Templates::UnqualifiedOf<TRecord>>::Type;

    /// \brief Generates a sequence that can be used to enumerate all
    ///        elements in a record.
    template <typename TRecord,
              template <typename> typename TRankTrait>
    using
    SequenceOf = Templates::MakeSequence<RankOf<TRecord, TRankTrait>>;

    /// \brief Helper function for ElementTypeListOf.
    template <template <Int, typename> typename TElementTypeTrait,
              typename TRecord,
              Int... TIndex>
    auto
    ElementTypeListOfHelper(Templates::Sequence<TIndex...>) noexcept
        -> Templates::TypeList<ElementTypeOf<TIndex,
                                             TRecord,
                                             TElementTypeTrait>...>;

    /// \brief List of types of a record's elements.
    template <typename TRecord,
              template <typename> typename TRankTrait,
              template <Int, typename> typename TElementTypeTrait>
    using
    ElementTypeListOf = decltype(
        ElementTypeListOfHelper<TElementTypeTrait,TRecord>(
            SequenceOf<TRecord, TRankTrait>{}));

    /// \brief Index of the first element with type TElement in a record.
    template <typename TElement,
              typename TRecord,
              template <typename> typename TRankTrait,
              template <Int, typename> typename TElementTypeTrait>
    inline constexpr Int
    ElementIndexOf = Templates::IndexOf<TElement,
                                        ElementTypeListOf<TRecord,
                                                          TRankTrait,
                                                          TElementTypeTrait>>;

}

// ===========================================================================
