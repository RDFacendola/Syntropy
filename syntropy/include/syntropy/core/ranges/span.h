
/// \file span.h
///
/// \brief This header is part of Syntropy core module.
/// It contains definition of spans.
///
/// \author Raffaele D. Facendola - Jun 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/core/support/compare.h"

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
        using TPointer = typename TTraits::TPointer;

        /// \brief Reference type.
        using TReference = typename TTraits::TReference;

        /// \brief Type of the span cardinality.
        using TCount = typename TTraits::TCount;

        /// \brief Create an empty span.
        constexpr
        BaseSpan() noexcept = default;

        /// \brief Create an empty span.
        constexpr
        BaseSpan(Null) noexcept;

        /// \brief Create a span given a pointer to the first element
        ///        and their number.
        constexpr
        BaseSpan(Immutable<TPointer> begin, Immutable<TCount> size) noexcept;

        /// \brief Create a span given a pointer to both the first and
        ///        past the last element.
        constexpr
        BaseSpan(Immutable<TPointer> begin, Immutable<TPointer> end) noexcept;

        /// \brief Converting copy constructor.
        template <typename UType, typename UTraits>
        constexpr
        BaseSpan(Immutable<BaseSpan<UType, UTraits>> rhs) noexcept;

        /// \brief Default destructor.
        ~BaseSpan() noexcept = default;

        /// \brief Copy assignment operator.
        template <typename UType, typename UTraits>
        constexpr Mutable<BaseSpan>
        operator=(Immutable<BaseSpan<UType, UTraits>> rhs) noexcept;

        /// \brief Check whether the span is non-empty.
        [[nodiscard]] constexpr explicit
        operator Bool() const noexcept;

        /// \brief Access an element by index.
        ///
        /// If the provided index is not within the BaseSpan the behavior
        /// of this method is undefined.
        [[nodiscard]] constexpr TReference
        operator[](Immutable<TCount> index) const noexcept;

        /// \brief Access the underlying storage.
        [[nodiscard]] constexpr TPointer
        GetData() const noexcept;

        /// \brief Get the number of elements in the span.
        [[nodiscard]] constexpr Immutable<TCount>
        GetCount() const noexcept;

    private:

        /// \brief Pointer to the first element in the range.
        TPointer data_{ nullptr };

        /// \brief Number of elements in the span.
        TCount  count_{};

    };

    /************************************************************************/
    /* SPAN                                                                 */
    /************************************************************************/

    /// \brief Traits for read-only spans.
    template <typename TType>
    struct SpanTraits
    {
        /// \brief Pointer type.
        using TPointer = Ptr<TType>;

        /// \brief Reference type.
        using TReference = Immutable<TType>;

        /// \brief Cardinality type.
        using TCount = Int;
    };

    /// \brief Represents a span of read-only elements.
    template <typename TType>
    using Span = BaseSpan<TType, SpanTraits<TType>>;

    /************************************************************************/
    /* RW SPAN                                                              */
    /************************************************************************/

    /// \brief Traits for read-write spans.
    template <typename TType>
    struct RWSpanTraits
    {
        /// \brief Pointer type.
        using TPointer = RWPtr<TType>;

        /// \brief Reference type.
        using TReference = Mutable<TType>;

        /// \brief Cardinality type.
        using TCount = Int;
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

    /// \brief Create a new span by deducing template from arguments.
    template <typename TType, typename TCount>
    [[nodiscard]] constexpr Span<TType>
    MakeSpan(Ptr<TType> begin, Immutable<TCount> size) noexcept;

    /// \brief Create a new span by deducing template from arguments.
    template <typename TType>
    [[nodiscard]] constexpr Span<TType>
    MakeSpan(Ptr<TType> begin, Ptr<TType> end) noexcept;

    /// \brief Create a new span by deducing template from arguments.
    template <typename TType, typename TCount>
    [[nodiscard]] constexpr RWSpan<TType>
    MakeSpan(RWPtr<TType> begin, Immutable<TCount> size) noexcept;

    /// \brief Create a new span by deducing template from arguments.
    template <typename TType>
    [[nodiscard]] constexpr RWSpan<TType>
    MakeSpan(RWPtr<TType> begin, RWPtr<TType> end) noexcept;

    /// \brief Create a new span by deducing template from arguments.
    template <typename TType, Int VSize>
    [[nodiscard]] constexpr auto
    MakeSpan(TType (&rhs)[VSize]) noexcept;

}

// ===========================================================================

#include "details/span.inl"

// ===========================================================================
