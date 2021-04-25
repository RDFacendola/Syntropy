
/// \file span.h
///
/// \brief This header is part of Syntropy core module.
/// It contains definition of spans.
///
/// \author Raffaele D. Facendola - Jun 2020

// ===========================================================================

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/core/comparisons/compare.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* BASE SPAN                                                            */
    /************************************************************************/

    /// \brief Represents a contiguous, non-owning, range of elements.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TType>
    class BaseSpan
    {
        template <typename UType>
        friend class BaseSpan;

    public:

        /// \brief Pointer type.
        using PointerType = BasePtr<TType>;

        /// \brief Reference type.
        using ReferenceType = Reference<TType>;

        /// \brief Create an empty span.
        constexpr
        BaseSpan() noexcept = default;

        /// \brief Create an empty span.
        constexpr
        BaseSpan(Null) noexcept;

        /// \brief Create a span given a pointer to the first element and their
        ///        count.
        constexpr
        BaseSpan(PointerType begin, Int count) noexcept;

        /// \brief Create a span given a pointer to both the first and past the
        ///        last element.
        constexpr
        BaseSpan(PointerType begin, PointerType nd) noexcept;

        /// \brief Converting copy-constructor.
        template <typename UType>
        constexpr
        BaseSpan(Immutable<BaseSpan<UType>> rhs) noexcept;

        /// \brief Default destructor.
        ~BaseSpan() noexcept = default;

        /// \brief Copy-assignment operator.
        template <typename UType>
        constexpr Mutable<BaseSpan>
        operator=(Immutable<BaseSpan<UType>> rhs) noexcept;

        /// \brief Check whether the span is non-empty.
        [[nodiscard]] constexpr explicit
        operator Bool() const noexcept;

        /// \brief Access an element by index.
        ///
        /// \remarks Undefined behavior if range boundaries are exceeded.
        [[nodiscard]] constexpr ReferenceType
        operator[](Int index) const noexcept;

        /// \brief Access the underlying storage.
        [[nodiscard]] constexpr PointerType
        GetData() const noexcept;

        /// \brief Get the number of elements in the span.
        [[nodiscard]] constexpr Int
        GetCount() const noexcept;

        /// \brief Select a subrange of elements.
        ///
        /// \remarks Undefined behavior if range boundaries are exceeded.
        [[nodiscard]] constexpr BaseSpan
        Select(Int offset, Int count) const noexcept;

    private:

        /// \brief Pointer to the first element in the range.
        PointerType data_{ nullptr };

        /// \brief Number of elements in the span.
        Int count_{};

    };

    /// \brief Concept for template arguments that bind to spans only.
    template <typename TSpan>
    concept IsSpan
        = Templates::IsTemplateSpecializationOf<TSpan, BaseSpan>;

    /************************************************************************/
    /* SPAN                                                                 */
    /************************************************************************/

    /// \brief A span of read-only elements.
    template <typename TType>
    using Span = BaseSpan<Templates::ReadOnlyOf<TType>>;

    /************************************************************************/
    /* RW SPAN                                                              */
    /************************************************************************/

    /// \brief A span of read-write elements.
    template <typename TType>
    using RWSpan = BaseSpan<Templates::ReadWriteOf<TType>>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // BaseSpan.
    // =========

    /// \brief Check whether lhs and rhs are equivalent.
    template <IsSpan TSpan, IsSpan USpan>
    [[nodiscard]] constexpr Bool
    operator==(Immutable<TSpan> lhs, Immutable<USpan> rhs) noexcept;

    /// \brief Compare two spans lexicographically.
    template <IsSpan TSpan, IsSpan USpan>
    [[nodiscard]] constexpr Ordering
    operator<=>(Immutable<TSpan> lhs, Immutable<USpan> rhs) noexcept;

    // Access.
    // =======

    /// \brief Convert rhs to a read-only span.
    template <typename TType>
    [[nodiscard]] constexpr Span<TType>
    ToReadOnly(Immutable<BaseSpan<TType>> rhs) noexcept;

    /// \brief Convert rhs to a read-write span.
    /// \remarks If the original span is not read-writable, accessing the
    ///          returned values results in undefined behavior.
    template <typename TType>
    [[nodiscard]] constexpr RWSpan<TType>
    ToReadWrite(Immutable<BaseSpan<TType>> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a read-only span by deducing template from arguments.
    template <typename TType>
    [[nodiscard]] constexpr Span<TType>
    MakeSpan(Ptr<TType> begin, Int count) noexcept;

    /// \brief Create a read-only span by deducing template from arguments.
    template <typename TType>
    [[nodiscard]] constexpr Span<TType>
    MakeSpan(Ptr<TType> begin, Ptr<TType> end) noexcept;

    /// \brief Create a read-write span by deducing template from arguments.
    template <typename TType>
    [[nodiscard]] constexpr RWSpan<TType>
    MakeSpan(RWPtr<TType> begin, Int count) noexcept;

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
