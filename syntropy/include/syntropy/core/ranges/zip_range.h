
/// \file zip_range.h
///
/// \brief This header is part of the Syntropy core module. It contains
///        definitions for adapters used to zip multiple range_views together.
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

    /// \brief Adapter class used to tie many range views together.
    ///
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

        /// \brief Create a new range view by zipping together one or more
        ///        range views.
        constexpr
        ZipRange(Immutable<TRangeViews>... range_views) noexcept;

        /// \brief Default destructor.
        ~ZipRange() noexcept = default;

        /// \brief Default copy-assignment operator.
        constexpr Mutable<ZipRange>
        operator=(Immutable<ZipRange> range_view) noexcept = default;

        /// \brief Access the first element in the view.
        ///
        /// \remarks Undefined behavior if the view is empty.
        [[nodiscard]] constexpr decltype(auto)
        GetFront() const noexcept;

        /// \brief Discard the first element in the view and return a
        ///        view to the remaining elements.
        ///
        /// \remarks Undefined behavior if the view is empty.
        [[nodiscard]] constexpr auto
        PopFront() const noexcept;

        /// \brief Check whether the view is empty.
        [[nodiscard]] constexpr Bool
        IsEmpty() const noexcept;

        /// \brief Get the number of element.
        [[nodiscard]] constexpr auto
        GetCount() const noexcept;

        /// \brief Access the last element in the view.
        ///
        /// \remarks Undefined behavior if the view is empty.
        [[nodiscard]] constexpr decltype(auto)
        GetBack() const noexcept;

        /// \brief Discard the last element in the view and return the
        ///        view to the remaining elements.
        ///
        /// \remarks Undefined behavior if the view is empty.
        [[nodiscard]] constexpr auto
        PopBack() const noexcept;

        /// \brief Access a range view element by index.
        ///
        /// \remarks Undefined behavior if range view boundaries are exceeded.
        template <typename TIndex>
        [[nodiscard]] constexpr decltype(auto)
        At(Immutable<TIndex> index) const noexcept;

        /// \brief Obtain a sub-range-view.
        ///
        /// \remarks Undefined behavior if range view boundaries are exceeded.
        template <typename TIndex, typename TCount>
        [[nodiscard]] constexpr auto
        Slice(Immutable<TIndex> index, Immutable<TCount> count) const noexcept;

        /// \brief Access the storage of all tied range views.
        ///
        /// \remarks Undefined behavior if the view is empty.
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
    ///        provided arguments.
    template <Syntropy::Tuples::Concepts::NTuple TTuple>
    [[nodiscard]] constexpr auto
    MakeZipRangeFromTuple(Immutable<TTuple> range_views) noexcept;

    /// \brief Unzip a zip range view, producing a tuple containing the
    ///        individual range views.
    ///
    /// \remarks If the provided range view is not a ZipRange, the results is a
    ///          tuple with a single element.
    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<ZipRange<TRangeViews...>> range_view) noexcept;

    /// \brief Unzip a zip range view, producing a tuple containing the
    ///        individual range views.
    ///
    /// \remarks If the provided range view is not a ZipRange, the results is a
    ///          tuple with a single element.
    template <Concepts::ForwardRangeView TRange>
    [[nodiscard]] constexpr auto
    Unzip(Immutable<TRange> range_view) noexcept;

    /// \brief Create a new ZipRange by element-wise joining different range
    ///        views, flattening eventual ZipRanges on the first level.
    template <Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr ZipRange<TRangeViews...>
    Zip(Immutable<TRangeViews>... range_views) noexcept;

    /// \brief Access a zip range view by index.
    ///
    /// \remarks Ill-formed if the range view rank is exceeded.
    template <Int VIndex, Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immutable<ZipRange<TRangeViews...>> range_view) noexcept;

    /// \brief Access a zip range view by index.
    ///
    /// \remarks Ill-formed if the range view rank is exceeded.
    template <Int VIndex, Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr decltype(auto)
    Get(Mutable<ZipRange<TRangeViews...>> range_view) noexcept;

    /// \brief Access a zip range view by index.
    ///
    /// \remarks Ill-formed if the range view rank is exceeded.
    template <Int VIndex, Concepts::ForwardRangeView... TRangeViews>
    [[nodiscard]] constexpr decltype(auto)
    Get(Immovable<ZipRange<TRangeViews...>> range_view) noexcept;

    /// \brief Access a zip range view by index.
    ///
    /// \remarks Ill-formed if the range view rank is exceeded.
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
    template <Int VIndex,
              Syntropy::Ranges::Concepts::ForwardRangeView... TRangeViews>
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
