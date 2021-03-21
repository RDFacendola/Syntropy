
/// \file range.details.inl
///
/// \author Raffaele D. Facendola - March 2021

#pragma once

// ===========================================================================

namespace Syntropy::Ranges::Details
{
    /************************************************************************/
    /* RANGE VIEW                                                           */
    /************************************************************************/

    template <typename TRangeView, typename TCardinality>
    constexpr
    RangeView<TRangeView, TCardinality>
    ::RangeView(Immutable<TRangeView> range_view,
                Immutable<TCardinality> offset,
                Immutable<TCardinality> count) noexcept
        : range_view_(range_view)
        , offset_(offset)
        , count_(count)
    {

    }

    template <typename TRangeView, typename TCardinality>
    [[nodiscard]] constexpr Immutable<TCardinality>
    RangeView<TRangeView, TCardinality>
    ::GetCount() const noexcept
    {
        return count_;
    }

    template <typename TRangeView, typename TCardinality>
    [[nodiscard]] constexpr decltype(auto)
    RangeView<TRangeView, TCardinality>
    ::At(Immutable<TCardinality> index) const noexcept
    {
        return Details::RouteAt(index + offset_);
    }

    template <typename TRangeView, typename TCardinality>
    [[nodiscard]] constexpr RangeView<TRangeView, TCardinality>
    RangeView<TRangeView, TCardinality>
    ::Select(Immutable<TCardinality> offset,
             Immutable<TCardinality> count) const noexcept
    {
        return RangeView(range_view_, offset_ + offset, count);
    }

    template <typename TRangeView, typename TCardinality>
    template <typename TThis>
    [[nodiscard]] constexpr decltype(auto)
    RangeView<TRangeView, TCardinality>
    ::GetData() const noexcept
    {
        return Details::RouteData(range_view_) + offset_;
    }

    /************************************************************************/
    /* VIEW OF                                                              */
    /************************************************************************/

    template <typename TRange>
    inline auto
    InvokeViewOf(Forwarding<TRange> range, ExtensionPriority) noexcept
        -> decltype(Extensions::ViewOf<TRange>{}(Forward<TRange>(range)))
    {
        return Extensions::ViewOf<TRange>{}(Forward<TRange>(range));
    }

    template <typename TRange>
    inline auto
    InvokeViewOf(Forwarding<TRange> range, MemberFunctionPriority) noexcept
        -> decltype(range.GetView())
    {
        return range.GetView();
    }

    template <typename TRange>
    inline auto
    InvokeViewOf(Forwarding<TRange> range, NonMemberFunctionPriority) noexcept
        -> decltype(ViewOf(Forward<TRange>(range)))
    {
        return ViewOf(Forward<TRange>(range));
    }

    template <typename TRange>
    inline auto
    InvokeViewOf(Forwarding<TRange> range, FallbackPriority) noexcept
        -> decltype(range)
    {
        return range;
    }

    template <typename TRange>
    inline auto
    RouteViewOf(Forwarding<TRange> range) noexcept
        -> decltype(InvokeViewOf(Forward<TRange>(range), kMaxPriority))
    {
        return InvokeViewOf(Forward<TRange>(range), kMaxPriority);
    }

    /************************************************************************/
    /* COUNT                                                                */
    /************************************************************************/

    template <typename TRangeView>
    inline auto
    InvokeCount(Immutable<TRangeView> range_view, ExtensionPriority) noexcept
        -> decltype(Extensions::Count<TRangeView>{}(range_view))
    {
        return Extensions::Count<TRangeView>{}(range_view);
    }

    template <typename TRangeView>
    inline auto
    InvokeCount(Immutable<TRangeView> range_view, MemberFunctionPriority)
        noexcept -> decltype(range_view.GetCount())
    {
        return range_view.GetCount();
    }

    template <typename TRangeView>
    inline auto
    InvokeCount(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
        noexcept -> decltype(Count(range_view))
    {
        return Count(range_view);
    }

    template <typename TRangeView>
    inline auto
    RouteCount(Immutable<TRangeView> range_view) noexcept
        -> decltype(InvokeCount(range_view, kMaxPriority))
    {
        return InvokeCount(range_view, kMaxPriority);
    }

    /************************************************************************/
    /* IS EMPTY                                                             */
    /************************************************************************/

    template <typename TRangeView>
    inline auto
    InvokeIsEmpty(Immutable<TRangeView> range_view, ExtensionPriority) noexcept
        -> decltype(Extensions::IsEmpty<TRangeView>{}(range_view))
    {
        return Extensions::IsEmpty<TRangeView>{}(range_view);
    }

    template <typename TRangeView>
    inline auto
    InvokeIsEmpty(Immutable<TRangeView> range_view, MemberFunctionPriority)
        noexcept -> decltype(range_view.IsEmpty())
    {
        return range_view.IsEmpty();
    }

    template <typename TRangeView>
    inline auto
    InvokeIsEmpty(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
        noexcept -> decltype(IsEmpty(range_view))
    {
        return IsEmpty(range_view);
    }

    template <typename TRangeView>
    inline auto
    InvokeIsEmpty(Immutable<TRangeView> range_view, FallbackPriority) noexcept
        -> decltype(Details::RouteCount(range_view) ==
                    RangeCardinalityTypeOf<TRangeView>{})
    {
        return Details::RouteCount(range_view) ==
            RangeCardinalityTypeOf<TRangeView>{};
    }

    template <typename TRangeView>
    inline auto
    RouteIsEmpty(Immutable<TRangeView> range_view) noexcept
        -> decltype(InvokeIsEmpty(range_view, kMaxPriority))
    {
        return InvokeIsEmpty(range_view, kMaxPriority);
    }

    /************************************************************************/
    /* DATA                                                                 */
    /************************************************************************/

    template <typename TRangeView>
    inline auto
    InvokeData(Immutable<TRangeView> range_view, ExtensionPriority) noexcept
        -> decltype(Extensions::Data<TRangeView>{}(range_view))
    {
        return Extensions::Data<TRangeView>{}(range_view);
    }

    template <typename TRangeView>
    inline auto
    InvokeData(Immutable<TRangeView> range_view, MemberFunctionPriority)
       noexcept -> decltype(range_view.GetData())
    {
        return range_view.GetData();
    }

    template <typename TRangeView>
    inline auto
    InvokeData(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
       noexcept -> decltype(Data(range_view))
    {
        return Data(range_view);
    }

    template <typename TRangeView>
    inline auto
    RouteData(Immutable<TRangeView> range_view)
        noexcept -> decltype(InvokeData(range_view, kMaxPriority))
    {
        return InvokeData(range_view, kMaxPriority);
    }

    /************************************************************************/
    /* AT                                                                   */
    /************************************************************************/

    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             ExtensionPriority) noexcept
        -> decltype(Extensions::At<TRangeView>{}(range_view, index))
    {
        return Extensions::At<TRangeView>{}(range_view, index);
    }

    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             MemberFunctionPriority) noexcept
        -> decltype(range_view.At(index))
    {
        return range_view.At(index);
    }

    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             MemberOperatorPriority) noexcept
        -> decltype(range_view[index])
    {
        return range_view[index];
    }

    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             NonMemberFunctionPriority) noexcept
        -> decltype(At(range_view, index))
    {
        return At(range_view, index);
    }

    template <typename TRangeView, typename TIndex>
    inline auto
    InvokeAt(Immutable<TRangeView> range_view,
             Immutable<TIndex> index,
             FallbackPriority) noexcept
        -> decltype(Details::RouteData(range_view)[index])
    {
        return Details::RouteData(range_view)[index];
    }

    template <typename TRangeView, typename TIndex>
    inline auto
    RouteAt(Immutable<TRangeView> range_view, Immutable<TIndex> index) noexcept
        -> decltype(InvokeAt(range_view, index, kMaxPriority))
    {
        return InvokeAt(range_view, index, kMaxPriority);
    }

    /************************************************************************/
    /* SELECT                                                               */
    /************************************************************************/

    template <typename TRangeView, typename TCardinality>
    inline auto
    InvokeSelect(Immutable<TRangeView> range_view,
                 Immutable<TCardinality> offset,
                 Immutable<TCardinality> count,
                 ExtensionPriority) noexcept
        -> decltype(Extensions::Select<TRangeView>{}(range_view,
                                                     offset,
                                                     count))
    {
        return Extensions::Select<TRangeView>{}(range_view, offset, count);
    }

    template <typename TRangeView, typename TCardinality>
    inline auto
    InvokeSelect(Immutable<TRangeView> range_view,
                 Immutable<TCardinality> offset,
                 Immutable<TCardinality> count,
                 MemberFunctionPriority)
        noexcept -> decltype(range_view.Select(offset, count))
    {
        return range_view.Select(offset, count);
    }

    template <typename TRangeView, typename TCardinality>
    inline auto
    InvokeSelect(Immutable<TRangeView> range_view,
                 Immutable<TCardinality> offset,
                 Immutable<TCardinality> count,
                 NonMemberFunctionPriority)
        noexcept -> decltype(Select(range_view, offset, count))
    {
        return Select(range_view, offset, count);
    }

    template <typename TRangeView, typename TCardinality>
    inline auto
    InvokeSelect(Immutable<TRangeView> range_view,
                 Immutable<TCardinality> offset,
                 Immutable<TCardinality> count,
                 FallbackPriority) noexcept
        -> decltype(RangeView(range_view, offset, count))
    {
        return RangeView(range_view, offset, count);
    }

    template <typename TRangeView, typename TCardinality>
    inline auto
    RouteSelect(Immutable<TRangeView> range_view,
                Immutable<TCardinality> offset,
                Immutable<TCardinality> count) noexcept
        -> decltype(InvokeSelect(range_view, offset, count, kMaxPriority))
    {
        return InvokeSelect(range_view, offset, count, kMaxPriority);
    }

    /************************************************************************/
    /* FRONT                                                                */
    /************************************************************************/

    template <typename TRangeView>
    inline auto
    InvokeFront(Immutable<TRangeView> range_view, ExtensionPriority) noexcept
        -> decltype(Extensions::Front<TRangeView>{}(range_view))
    {
        return Extensions::Front<TRangeView>{}(range_view);
    }

    template <typename TRangeView>
    inline auto
    InvokeFront(Immutable<TRangeView> range_view, MemberFunctionPriority)
        noexcept -> decltype(range_view.GetFront())
    {
        return range_view.GetFront();
    }

    template <typename TRangeView>
    inline auto
    InvokeFront(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
        noexcept -> decltype(Front(range_view))
    {
        return Front(range_view);
    }

    template <typename TRangeView>
    inline auto
    InvokeFront(Immutable<TRangeView> range_view, FallbackPriority) noexcept
        -> decltype(Details::RouteAt(range_view,
                                     RangeCardinalityTypeOf<TRangeView>{}))
    {
        return Details::RouteAt(range_view,
                                RangeCardinalityTypeOf<TRangeView>{});
    }

    template <typename TRangeView>
    inline auto
    RouteFront(Immutable<TRangeView> range_view) noexcept
        -> decltype(InvokeFront(range_view, kMaxPriority))
    {
        return InvokeFront(range_view, kMaxPriority);
    }

    /************************************************************************/
    /* BACK                                                                 */
    /************************************************************************/

    template <typename TRangeView>
    inline auto
    InvokeBack(Immutable<TRangeView> range_view, ExtensionPriority) noexcept
        -> decltype(Extensions::Back<TRangeView>{}(range_view))
    {
        return Extensions::Back<TRangeView>{}(range_view);
    }

    template <typename TRangeView>
    inline auto
    InvokeBack(Immutable<TRangeView> range_view, MemberFunctionPriority)
        noexcept -> decltype(range_view.GetBack())
    {
        return range_view.GetBack();
    }

    template <typename TRangeView>
    inline auto
    InvokeBack(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
        noexcept -> decltype(Back(range_view))
    {
        return Back(range_view);
    }

    template <typename TRangeView>
    inline auto
    InvokeBack(Immutable<TRangeView> range_view, FallbackPriority) noexcept
        -> decltype(Details::RouteAt(
            range_view,
            Details::RouteCount(range_view) -
                RangeCardinalityTypeOf<TRangeView>{ 1 }))
    {
        return Details::RouteAt(
            range_view,
            Details::RouteCount(range_view) -
                RangeCardinalityTypeOf<TRangeView>{ 1 });
    }

    template <typename TRangeView>
    inline auto
    RouteBack(Immutable<TRangeView> range_view) noexcept
        -> decltype(InvokeBack(range_view, kMaxPriority))
    {
        return InvokeBack(range_view, kMaxPriority);
    }

    /************************************************************************/
    /* POP FRONT                                                            */
    /************************************************************************/

    template <typename TRangeView>
    inline auto
    InvokePopFront(Immutable<TRangeView> range_view, ExtensionPriority)
        noexcept -> decltype(Extensions::PopFront<TRangeView>{}(range_view))
    {
        return Extensions::PopFront<TRangeView>{}(range_view);
    }

    template <typename TRangeView>
    inline auto
    InvokePopFront(Immutable<TRangeView> range_view, MemberFunctionPriority)
        noexcept -> decltype(range_view.PopFront())
    {
        return range_view.PopFront();
    }

    template <typename TRangeView>
    inline auto
    InvokePopFront(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
        noexcept -> decltype(PopFront(range_view))
    {
        return PopFront(range_view);
    }

    template <typename TRangeView>
    inline auto
    InvokePopFront(Immutable<TRangeView> range_view, FallbackPriority) noexcept
        -> decltype(Details::RouteSelect(
            range_view,
            RangeCardinalityTypeOf<TRangeView>{ 1 },
            Details::RouteCount(range_view) -
                RangeCardinalityTypeOf<TRangeView>{ 1 }))
    {
        return Details::RouteSelect(
            range_view,
            RangeCardinalityTypeOf<TRangeView>{ 1 },
            Details::RouteCount(range_view) -
                RangeCardinalityTypeOf<TRangeView>{ 1 });
    }

    template <typename TRangeView>
    inline auto
    RoutePopFront(Immutable<TRangeView> range_view) noexcept
        -> decltype(InvokePopFront(range_view, kMaxPriority))
    {
        return InvokePopFront(range_view, kMaxPriority);
    }

    /************************************************************************/
    /* POP BACK                                                             */
    /************************************************************************/

    template <typename TRangeView>
    inline auto
    InvokePopBack(Immutable<TRangeView> range_view, ExtensionPriority) noexcept
        -> decltype(Extensions::PopBack<TRangeView>{}(range_view))
    {
        return Extensions::PopBack<TRangeView>{}(range_view);
    }

    template <typename TRangeView>
    inline auto
    InvokePopBack(Immutable<TRangeView> range_view, MemberFunctionPriority)
        noexcept -> decltype(range_view.PopBack())
    {
        return range_view.PopBack();
    }

    template <typename TRangeView>
    inline auto
    InvokePopBack(Immutable<TRangeView> range_view, NonMemberFunctionPriority)
       noexcept -> decltype(PopBack(range_view))
    {
        return PopBack(range_view);
    }

    template <typename TRangeView>
    inline auto
    InvokePopBack(Immutable<TRangeView> range_view, FallbackPriority) noexcept
        -> decltype(Details::RouteSelect(
            range_view,
            RangeCardinalityTypeOf<TRangeView>{ 0 },
            Details::RouteCount(range_view)
                - RangeCardinalityTypeOf<TRangeView>{ 1 }))
    {
        return Details::RouteSelect(
            range_view,
            RangeCardinalityTypeOf<TRangeView>{ 0 },
            Details::RouteCount(range_view)
                - RangeCardinalityTypeOf<TRangeView>{ 1 });
    }

    template <typename TRangeView>
    inline auto
    RoutePopBack(Immutable<TRangeView> range_view) noexcept
        -> decltype(InvokePopBack(range_view, kMaxPriority))
    {
        return InvokePopBack(range_view, kMaxPriority);
    }

}

// ===========================================================================
