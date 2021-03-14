
/// \file zip_range.h
///
/// \brief his header is part of the Syntropy core module. It contains
/// definitions for adapters used to zip multiple range_views together.
///
/// Ranges specifications based on the awesome
/// https://www.slideshare.net/rawwell/iteratorsmustgo
///
/// \author Raffaele D. Facendola - Nov 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/tuples/ntuple.h"
#include "syntropy/core/ranges/forward_range.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* ZIP RANGE VIEW                                                       */
    /************************************************************************/

    /// \brief Adapter class used to zip many range_views together.
    ///
    /// The new range_view has elements equal to the Tuple consisting of each
    /// element in each source range_view, tied together.
    /// \author Raffaele D. Facendola - November 2020.
    /// \author Raffaele D. Facendola - January 2021.
    template <Concepts::ForwardRangeView... TRangeViews>
    class ZipRange
    {
        template <Concepts::ForwardRangeView... URangeViews>
        friend constexpr auto
        Unzip(Immutable<ZipRange<URangeViews...>> range_view) noexcept;

    public:

        /// \brief Create an empty zip-range_view.
        constexpr
        ZipRange() noexcept = default;

        /// \brief Default copy-constructor.
        constexpr
        ZipRange(Immutable<ZipRange> rhs) noexcept = default;

        /// \brief Create a new range_view by zipping together one or more range_views.
        constexpr
        ZipRange(Immutable<TRangeViews>... range_views) noexcept;

        /// \brief Default destructor.
        ~ZipRange() noexcept = default;

        /// \brief Default copy-assignment operator.
        constexpr Mutable<ZipRange>
        operator=(Immutable<ZipRange> range_view) noexcept = default;

        /// \brief Access range_view's first element.
        ///
        /// \remarks Accessing the first element of an empty range_view results
        ///          in undefined behavior.
        [[nodiscard]] constexpr decltype(auto)
        GetFront() const noexcept;

        /// \brief Discard range_view's first element and return the resulting
        ///        range_view.
        ///
        /// \remarks If the provided range_view is empty, the behavior of this
        ///          method is undefined.
        [[nodiscard]] constexpr auto
        PopFront() const noexcept;

        /// \brief Check whether the range_view is empty.
        [[nodiscard]] constexpr Bool
        IsEmpty() const noexcept;

        /// \brief Get range_view's elements count.
        [[nodiscard]] constexpr auto
        GetCount() const noexcept;

        /// \brief Access range_view's last element.
        ///
        /// \remarks Accessing the last element of an empty range_view results in
        ///          undefined behavior.
        [[nodiscard]] constexpr decltype(auto)
        GetBack() const noexcept;

        /// \brief Discard range_view's last element and return the resulting range_view.
        ///
        /// \remarks If the provided range_view is empty, the behavior of this
        ///          method is undefined.
        [[nodiscard]] constexpr auto
        PopBack() const noexcept;

        /// \brief Access range_view's element by index.
        ///
        /// \remarks Exceeding range_view boundaries results in undefined behavior.
        template <typename TIndex>
        [[nodiscard]] constexpr decltype(auto)
        At(Immutable<TIndex> index) const noexcept;

        /// \brief Obtain a view to a sub-range_view.
        ///
        /// \remarks Exceeding range_view boundaries results in undefined behavior.
        template <typename TIndex, typename TCount>
        [[nodiscard]] constexpr auto
        Slice(Immutable<TIndex> index, Immutable<TCount> count) const noexcept;

        /// \brief Access range_view's element storage.
        ///
        /// \remarks Accessing an empty range_view's storage results in
        ///          undefined behavior.
        [[nodiscard]] constexpr auto
        GetData() const noexcept;

    private:

        /// \brief Underlying range_views.
        Tuples::Tuple<TRangeViews...> range_views_;

    };

    /// \brief Deduction rule.
    template <Concepts::ForwardRangeView... TRangeViews>
    ZipRange(Immutable<TRangeViews>...) -> ZipRange<TRangeViews...>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new ZipRange by deducing templates types
    ///        from arguments.
    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr ZipRange<TRangeViews...>
    MakeZipRange(Immutable<TRangeViews>... range_views) noexcept;

    /// \brief Create a new ZipRange by deducing templates types from
    ///        arguments provided
    template <Syntropy::Tuples::Concepts::NTuple TTuple>
    [[nodiscard]] constexpr auto
    MakeZipRangeFromTuple(Immutable<TTuple> range_views) noexcept;

    /// \brief Access the individual range_views in a zip range_view.
    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<ZipRange<TRangeViews...>> range_view) noexcept;

    /// \brief Unzip a range_view, producing a tuple containing the individual
    ///        range_views.
    ///
    /// \remarks If the provided range_view is not a Zip-range_view, the results is a
    ///          tuple with a single element.
    template <Concepts::ForwardRangeView TRange>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<TRange> range_view) noexcept;

    /// \brief Create a new range_view by element-wise joining different range_views and
    ///        flattening zip-range_views on the first level.
    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr ZipRange<TRangeViews...>
    Zip(Immutable<TRangeViews>... range_views) noexcept;

    /// \brief
    /// Access the VIndex-th range_view in a zip-range_view.
    ///
    /// \remarks
    /// The program is ill-formed if no such range_view exists.
    template <Int VIndex, Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immutable<ZipRange<TRangeViews...>> range_view) noexcept;

    /// \brief
    /// Access the VIndex-th range_view in a zip-range_view.
    ///
    /// \remarks
    /// The program is ill-formed if no such range_view exists.
    template <Int VIndex, Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr decltype(auto)
    Get(Mutable<ZipRange<TRangeViews...>> range_view) noexcept;

    /// \brief
    /// Access the VIndex-th range_view in a zip-range_view.
    ///
    /// \remarks
    /// The program is ill-formed if no such range_view exists.
    template <Int VIndex, Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immovable<ZipRange<TRangeViews...>> range_view) noexcept;

    /// \brief
    /// Access the VIndex-th range_view in a zip-range_view.
    ///
    /// \remarks
    /// The program is ill-formed if no such range_view exists.
    template <Int VIndex, Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr decltype(auto)
    Get(Movable<ZipRange<TRangeViews...>> range_view) noexcept;

}

// ===========================================================================

namespace Syntropy::Tuples::Templates
{
    /************************************************************************/
    /* NTUPLE TRAITS                                                        */
    /************************************************************************/

    /// \brief Partial template specialization for tuples.
    template <Int VIndex, Syntropy::Ranges::Concepts::ForwardRangeView... TRangeViews>
    struct ElementTypeTraits<VIndex,
                             Syntropy::Ranges::ZipRange<TRangeViews...>>
        : Syntropy::Templates::Alias<ElementType<VIndex,
                                     Tuples::Tuple<TRangeViews...>>> {};

    /// \brief Partial template specialization for tuples.
    template <Syntropy::Ranges::Concepts::ForwardRangeView... TRangeViews>
    struct RankTypeTraits<Syntropy::Ranges::ZipRange<TRangeViews...>>
        : Syntropy::Templates::IntConstant<sizeof...(TRangeViews)> {};
}

// ===========================================================================

#include "details/zip_range.inl"

// ===========================================================================
