
/// \file range_view.details.h
///
/// \author Raffaele D. Facendola - March 2021

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/language/templates/priority.h"

#include "syntropy/core/ranges/range_extensions.h"

// ===========================================================================

namespace Syntropy::Ranges::Details
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
    /* VIEW OF                                                              */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRange>
    inline auto
    InvokeViewOf(Forwarding<TRange> range, ExtensionPriority) noexcept
        -> decltype(Extensions::ViewOf<TRange>{}(Forward<TRange>(range)));

    /// \brief Member-function.
    template <typename TRange>
    inline auto
    InvokeViewOf(Forwarding<TRange> range, MemberFunctionPriority) noexcept
        -> decltype(range.GetView());

    /// \brief Non-member function, possibly using ADL.
    template <typename TRange>
    inline auto
    InvokeViewOf(Forwarding<TRange> range, NonMemberFunctionPriority) noexcept
        -> decltype(ViewOf(Forward<TRange>(range)));

    /// \brief Fallback.
    template <typename TRange>
    inline auto
    InvokeViewOf(Forwarding<TRange> range, FallbackPriority) noexcept
        -> decltype(range);

    /// \brief Routes the invocation.
    template <typename TRange>
    inline auto
    RouteViewOf(Forwarding<TRange> range) noexcept
        -> decltype(InvokeViewOf(Forward<TRange>(range), kMaxPriority));

    /************************************************************************/
    /* COUNT                                                                */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRangeView>
    inline auto
    InvokeCount(Immutable<TRangeView> range_view, ExtensionPriority) noexcept
        -> decltype(Extensions::Count<TRangeView>{}(range_view));

    /// \brief Member-function.
    template <typename TRangeView>
    inline auto
    InvokeCount(Immutable<TRangeView> range_view, MemberFunctionPriority)
        noexcept -> decltype(range_view.GetCount());

    /// \brief Non-member function, possibly using ADL.
    template <typename TRangeView>
    inline auto
    InvokeCount(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
        noexcept -> decltype(Count(range_view));

    /// \brief Routes the invocation.
    template <typename TRangeView>
    inline auto
    RouteCount(Immutable<TRangeView> range_view) noexcept
        -> decltype(InvokeCount(range_view, kMaxPriority));

    /// \brief Type of a range's cardinality.
    template <typename TRangeView>
    using RangeCardinalityTypeOf = Templates::UnqualifiedOf<decltype(
        RouteCount(Templates::Declval<TRangeView>()))>;

    /************************************************************************/
    /* IS EMPTY                                                             */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRangeView>
    inline auto
    InvokeIsEmpty(Immutable<TRangeView> range_view, ExtensionPriority) noexcept
        -> decltype(Extensions::IsEmpty<TRangeView>{}(range_view));

    /// \brief Member-function.
    template <typename TRangeView>
    inline auto
    InvokeIsEmpty(Immutable<TRangeView> range_view, MemberFunctionPriority)
        noexcept -> decltype(range_view.IsEmpty());

    /// \brief Non-member function, possibly using ADL.
    template <typename TRangeView>
    inline auto
    InvokeIsEmpty(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
        noexcept -> decltype(IsEmpty(range_view));

    /// \brief Fallback.
    template <typename TRangeView>
    inline auto
    InvokeIsEmpty(Immutable<TRangeView> range_view, FallbackPriority) noexcept
        -> decltype(Details::RouteCount(range_view) ==
                    RangeCardinalityTypeOf<TRangeView>{});

    /// \brief Routes the invocation.
    template <typename TRangeView>
    inline auto
    RouteIsEmpty(Immutable<TRangeView> range_view) noexcept
        -> decltype(InvokeIsEmpty(range_view, kMaxPriority));

    /************************************************************************/
    /* DATA                                                                 */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRangeView>
    inline auto
    InvokeData(Immutable<TRangeView> range_view, ExtensionPriority) noexcept
        -> decltype(Extensions::Data<TRangeView>{}(range_view));

    /// \brief Member-function.
    template <typename TRangeView>
    inline auto
    InvokeData(Immutable<TRangeView> range_view, MemberFunctionPriority)
       noexcept -> decltype(range_view.GetData());

    /// \brief Non-member function, possibly using ADL.
    template <typename TRangeView>
    inline auto
    InvokeData(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
       noexcept -> decltype(Data(range_view));

    /// \brief Routes the invocation.
    template <typename TRangeView>
    inline auto
    RouteData(Immutable<TRangeView> range_view)
        noexcept -> decltype(InvokeData(range_view, kMaxPriority));

    /************************************************************************/
    /* AT                                                                   */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             ExtensionPriority) noexcept
        -> decltype(Extensions::At<TRangeView>{}(range_view, index));

    /// \brief Member-function.
    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             MemberFunctionPriority) noexcept
        -> decltype(range_view.At(index));

    /// \brief Member-operator.
    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             MemberOperatorPriority) noexcept
        -> decltype(range_view[index]);

    /// \brief Non-member function, possibly using ADL.
    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             NonMemberFunctionPriority) noexcept
        -> decltype(At(range_view, index));

    /// \brief Fallback.
    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             FallbackPriority) noexcept
        -> decltype(Details::RouteData(range_view)[index]);

    /// \brief Routes the invocation.
    template <typename TRangeView, typename TIndex>
    inline auto
    RouteAt(Immutable<TRangeView> range_view, Immutable<TIndex> index) noexcept
        -> decltype(InvokeAt(range_view, index, kMaxPriority));

    /************************************************************************/
    /* SELECT                                                               */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRangeView, typename TCardinality>
    inline auto
    InvokeSelect(Immutable<TRangeView> range_view,
                 Immutable<TCardinality> offset,
                 Immutable<TCardinality> count,
                 ExtensionPriority) noexcept
        -> decltype(Extensions::Select<TRangeView>{}(range_view,
                                                     offset,
                                                     count));

    /// \brief Member-function.
    template <typename TRangeView, typename TCardinality>
    inline auto
    InvokeSelect(Immutable<TRangeView> range_view,
                 Immutable<TCardinality> offset,
                 Immutable<TCardinality> count,
                 MemberFunctionPriority)
        noexcept -> decltype(range_view.Select(offset, count));

    /// \brief Non-member function, possibly using ADL.
    template <typename TRangeView, typename TCardinality>
    inline auto
    InvokeSelect(Immutable<TRangeView> range_view,
                 Immutable<TCardinality> offset,
                 Immutable<TCardinality> count,
                 NonMemberFunctionPriority)
        noexcept -> decltype(Select(range_view, offset, count));

    /// \brief Routes the invocation.
    template <typename TRangeView, typename TCardinality>
    inline auto
    RouteSelect(Immutable<TRangeView> range_view,
                Immutable<TCardinality> offset,
                Immutable<TCardinality> count) noexcept
        -> decltype(InvokeSelect(range_view, offset, count, kMaxPriority));

    /************************************************************************/
    /* FRONT                                                                */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRangeView>
    inline auto
    InvokeFront(Immutable<TRangeView> range_view, ExtensionPriority) noexcept
        -> decltype(Extensions::Front<TRangeView>{}(range_view));

    /// \brief Member-function.
    template <typename TRangeView>
    inline auto
    InvokeFront(Immutable<TRangeView> range_view, MemberFunctionPriority)
        noexcept -> decltype(range_view.GetFront());

    /// \brief Non-member function, possibly using ADL.
    template <typename TRangeView>
    inline auto
    InvokeFront(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
        noexcept -> decltype(Front(range_view));

    /// \brief Fallback.
    template <typename TRangeView>
    inline auto
    InvokeFront(Immutable<TRangeView> range_view, FallbackPriority) noexcept
        -> decltype(Details::RouteAt(range_view,
                                     RangeCardinalityTypeOf<TRangeView>{}));

    /// \brief Routes the invocation.
    template <typename TRangeView>
    inline auto
    RouteFront(Immutable<TRangeView> range_view) noexcept
        -> decltype(InvokeFront(range_view, kMaxPriority));

    /************************************************************************/
    /* BACK                                                                 */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRangeView>
    inline auto
    InvokeBack(Immutable<TRangeView> range_view, ExtensionPriority) noexcept
        -> decltype(Extensions::Back<TRangeView>{}(range_view));

    /// \brief Member-function.
    template <typename TRangeView>
    inline auto
    InvokeBack(Immutable<TRangeView> range_view, MemberFunctionPriority)
        noexcept -> decltype(range_view.GetBack());

    /// \brief Non-member function, possibly using ADL.
    template <typename TRangeView>
    inline auto
    InvokeBack(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
        noexcept -> decltype(Back(range_view));

    /// \brief Fallback.
    template <typename TRangeView>
    inline auto
    InvokeBack(Immutable<TRangeView> range_view, FallbackPriority) noexcept
        -> decltype(Details::RouteAt(range_view,
                                     --Details::RouteCount(range_view)));

    /// \brief Routes the invocation.
    template <typename TRangeView>
    inline auto
    RouteBack(Immutable<TRangeView> range_view) noexcept
        -> decltype(InvokeBack(range_view, kMaxPriority));

    /************************************************************************/
    /* POP FRONT                                                            */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRangeView>
    inline auto
    InvokePopFront(Immutable<TRangeView> range_view, ExtensionPriority)
        noexcept -> decltype(Extensions::PopFront<TRangeView>{}(range_view));

    /// \brief Member-function.
    template <typename TRangeView>
    inline auto
    InvokePopFront(Immutable<TRangeView> range_view, MemberFunctionPriority)
        noexcept -> decltype(range_view.PopFront());

    /// \brief Non-member function, possibly using ADL.
    template <typename TRangeView>
    inline auto
    InvokePopFront(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
        noexcept -> decltype(PopFront(range_view));

    /// \brief Fallback.
    template <typename TRangeView>
    inline auto
    InvokePopFront(Immutable<TRangeView> range_view, FallbackPriority) noexcept
        -> decltype(Details::RouteSelect(
            range_view,
            RangeCardinalityTypeOf<TRangeView>{ 1 },
            Details::RouteCount(range_view) -
                RangeCardinalityTypeOf<TRangeView>{ 1 }));

    /// \brief Routes the invocation.
    template <typename TRangeView>
    inline auto
    RoutePopFront(Immutable<TRangeView> range_view) noexcept
        -> decltype(InvokePopFront(range_view, kMaxPriority));

    /************************************************************************/
    /* POP BACK                                                             */
    /************************************************************************/

    /// \brief Custom extension.
    template <typename TRangeView>
    inline auto
    InvokePopBack(Immutable<TRangeView> range_view, ExtensionPriority) noexcept
        -> decltype(Extensions::PopBack<TRangeView>{}(range_view));

    /// \brief Member-function.
    template <typename TRangeView>
    inline auto
    InvokePopBack(Immutable<TRangeView> range_view, MemberFunctionPriority)
        noexcept -> decltype(range_view.PopBack());

    /// \brief Non-member function, possibly using ADL.
    template <typename TRangeView>
    inline auto
    InvokePopBack(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
       noexcept -> decltype(PopBack(range_view));

    /// \brief Fallback.
    template <typename TRangeView>
    inline auto
    InvokePopBack(Immutable<TRangeView> range_view, FallbackPriority) noexcept
        -> decltype(Details::RouteSelect(
            range_view,
            RangeCardinalityTypeOf<TRangeView>{ 0 },
            Details::RouteCount(range_view) -
                RangeCardinalityTypeOf<TRangeView>{ 1 }));

    /// \brief Routes the invocation.
    template <typename TRangeView>
    inline auto
    RoutePopBack(Immutable<TRangeView> range_view) noexcept
        -> decltype(InvokePopBack(range_view, kMaxPriority));

}

// ===========================================================================

#include "range.details.inl"

// ===========================================================================
