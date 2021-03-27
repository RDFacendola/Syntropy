
/// \file span.h
///
/// \brief This header is part of Syntropy core module.
/// It contains definition of spans.
///
/// \author Raffaele D. Facendola - Jun 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/core/algorithms/compare.h"

#include "syntropy/core/ranges/contiguous_range.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* BASE SPAN                                                            */
    /************************************************************************/

    /// \brief Represents a contiguous, non-owning, range of elements.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TType, typename TTraits>
    class BaseSpan
    {
        template <typename UType, typename UTraits>
        friend class BaseSpan;

    public:

        /// \brief Pointer type.
        using PointerType = typename TTraits::PointerType;

        /// \brief Reference type.
        using ReferenceType = typename TTraits::ReferenceType;

        /// \brief Type of the span cardinality.
        using CardinalityType = typename TTraits::CardinalityType;

        /// \brief Create an empty span.
        constexpr
        BaseSpan() noexcept = default;

        /// \brief Create an empty span.
        constexpr
        BaseSpan(Null) noexcept;

        /// \brief Create a span given a pointer to the first element and their
        ///        count.
        constexpr
        BaseSpan(Immutable<PointerType> begin,
                 Immutable<CardinalityType> count) noexcept;

        /// \brief Create a span given a pointer to both the first and past the
        ///        last element.
        constexpr
        BaseSpan(Immutable<PointerType> begin,
                 Immutable<PointerType> end) noexcept;

        /// \brief Converting copy-constructor.
        template <typename UType, typename UTraits>
        constexpr
        BaseSpan(Immutable<BaseSpan<UType, UTraits>> rhs) noexcept;

        /// \brief Default destructor.
        ~BaseSpan() noexcept = default;

        /// \brief Copy-assignment operator.
        template <typename UType, typename UTraits>
        constexpr Mutable<BaseSpan>
        operator=(Immutable<BaseSpan<UType, UTraits>> rhs) noexcept;

        /// \brief Check whether the span is non-empty.
        [[nodiscard]] constexpr explicit
        operator Bool() const noexcept;

        /// \brief Access an element by index.
        ///
        /// \remarks Undefined behavior if range boundaries are exceeded.
        [[nodiscard]] constexpr ReferenceType
        operator[](Immutable<CardinalityType> index) const noexcept;

        /// \brief Access the underlying storage.
        [[nodiscard]] constexpr PointerType
        GetData() const noexcept;

        /// \brief Get the number of elements in the span.
        [[nodiscard]] constexpr Immutable<CardinalityType>
        GetCount() const noexcept;

        /// \brief Select a subrange of elements.
        ///
        /// \remarks Undefined behavior if range boundaries are exceeded.
        [[nodiscard]] constexpr BaseSpan
        Select(Immutable<CardinalityType> offset,
               Immutable<CardinalityType> count) const noexcept;

    private:

        /// \brief Pointer to the first element in the range.
        PointerType data_{ nullptr };

        /// \brief Number of elements in the span.
        CardinalityType count_{};

    };

    /************************************************************************/
    /* BASE SPAN TRAITS                                                     */
    /************************************************************************/

    /// \brief Schema for base span traits.
    template <typename TPointer, typename TReference, typename TCardinality>
    struct BaseSpanTraits
    {
        /// \brief Pointer type.
        using PointerType = TPointer;

        /// \brief Reference type.
        using ReferenceType = TReference;

        /// \brief Cardinality type.
        using CardinalityType = TCardinality;
    };

    /************************************************************************/
    /* SPAN                                                                 */
    /************************************************************************/

    /// \brief Traits for read-only spans.
    template <typename TType>
    struct SpanTraits : BaseSpanTraits<Ptr<TType>, Immutable<TType>, Int>
    {

    };

    /// \brief Represents a span of read-only elements.
    template <typename TType>
    using Span = BaseSpan<TType, SpanTraits<TType>>;

    /************************************************************************/
    /* RW SPAN                                                              */
    /************************************************************************/

    /// \brief Traits for read-write spans.
    template <typename TType>
    struct RWSpanTraits : BaseSpanTraits<RWPtr<TType>, Mutable<TType>, Int>
    {

    };

    /// \brief Represents a span of read-write elements.
    template <typename TType>
    using RWSpan = BaseSpan<TType, RWSpanTraits<TType>>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // BaseSpan.
    // =========

    /// \brief Check whether lhs and rhs are equivalent.
    template <typename TType, typename TTraits,
              typename UType, typename UTraits>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<BaseSpan<TType, TTraits>> lhs,
               Immutable<BaseSpan<UType, UTraits>> rhs) noexcept;

    /// \brief Compare two spans lexicographically.
    template <typename TType, typename TTraits,
              typename UType, typename UTraits>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<BaseSpan<TType, TTraits>> lhs,
                Immutable<BaseSpan<UType, UTraits>> rhs) noexcept;

    // Access.
    // =======

    /// \brief Convert rhs to a read-only span.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr Span<TType>
    ToReadOnly(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept;

    /// \brief Convert rhs to a read-write span.
    /// \remarks If the original span is not read-writable, accessing the
    ///          returned values results in undefined behavior.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr RWSpan<TType>
    ToReadWrite(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a read-only span by deducing template from arguments.
    template <typename TType, typename TCardinality>
    [[nodiscard]] constexpr Span<TType>
    MakeSpan(Ptr<TType> begin, Immutable<TCardinality> count) noexcept;

    /// \brief Create a read-only span by deducing template from arguments.
    template <typename TType>
    [[nodiscard]] constexpr Span<TType>
    MakeSpan(Ptr<TType> begin, Ptr<TType> end) noexcept;

    /// \brief Create a read-write span by deducing template from arguments.
    template <typename TType, typename TCardinality>
    [[nodiscard]] constexpr RWSpan<TType>
    MakeSpan(RWPtr<TType> begin, Immutable<TCardinality> count) noexcept;

    /// \brief Create a read-write span by deducing template from arguments.
    template <typename TType>
    [[nodiscard]] constexpr RWSpan<TType>
    MakeSpan(RWPtr<TType> begin, RWPtr<TType> end) noexcept;

    /// \brief Create a span from a C array.
    template <typename TType, Int VCount>
    [[nodiscard]] constexpr auto
    MakeSpan(TType (&rhs)[VCount]) noexcept;

}

// ===========================================================================

#include "details/span.inl"

// ===========================================================================
