
/// \file span.h
/// \brief This header is part of Syntropy core module. It contains definition of spans.
///
/// \author Raffaele D. Facendola - Jun 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"
#include "syntropy/language/support/compare.h"

#include "syntropy/core/concepts/contiguous_range.h"

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* BASE SPAN                                                            */
    /************************************************************************/

    /// \brief Represents a contiguous, non-owning, range of elements.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TType, typename TTraits>
    class BaseSpan
    {
        template <typename TType, typename TTraits>
        friend constexpr Int Count(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept;

        template <typename TType, typename TTraits>
        friend constexpr typename TTraits::TPointer Data(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept;

    public:

        /// \brief Pointer type.
        using TPointer = typename TTraits::TPointer;

        /// \brief Reference type.
        using TReference = typename TTraits::TReference;

        /// \brief Create an empty span.
        constexpr BaseSpan() noexcept = default;

        /// \brief Create an empty span.
        constexpr BaseSpan(Null) noexcept;

        /// \brief Create a span given a pointer to the first element and their number.
        constexpr BaseSpan(Immutable<TPointer> begin, Int count) noexcept;

        /// \brief Create a span given a pointer to both the first and past the last element.
        constexpr BaseSpan(Immutable<TPointer> begin, Immutable<TPointer> end) noexcept;

        /// \brief Converting copy constructor.
        template <typename UType, typename UTraits>
        constexpr BaseSpan(Immutable<BaseSpan<UType, UTraits>> rhs) noexcept;

        /// \brief Default destructor.
        ~BaseSpan() noexcept = default;

        /// \brief Copy assignment operator.
        template <typename UType, typename UTraits>
        constexpr Mutable<BaseSpan> operator=(Immutable<BaseSpan<UType, UTraits>> rhs) noexcept;

        /// \brief Check whether the span is non-empty.
        [[nodiscard]] constexpr explicit operator Bool() const noexcept;

        /// \brief Access an element by index.
        /// If the provided index is not within the BaseSpan the behavior of this method is undefined.
        [[nodiscard]] constexpr TReference operator[](Int index) const noexcept;

    private:

        /// \brief Pointer to the first element in the range.
        TPointer data_{ nullptr };

        /// \brief Number of elements in the span.
        Int count_{ 0 };

    };

    /************************************************************************/
    /* SPAN                                                                 */
    /************************************************************************/

    /// \brief Traits for read-only spans.
    template <typename TType>
    struct SpanTypeTraits
    {
        /// \brief Pointer type.
        using TPointer = Ptr<TType>;

        /// \brief Reference type.
        using TReference = Immutable<TType>;
    };

    /// \brief Represents a span of read-only elements.
    template <typename TType>
    using Span = BaseSpan<TType, SpanTypeTraits<TType>>;

    /************************************************************************/
    /* RW SPAN                                                              */
    /************************************************************************/

    /// \brief Traits for read-write spans.
    template <typename TType>
    struct RWSpanTypeTraits
    {
        /// \brief Pointer type.
        using TPointer = RWPtr<TType>;

        /// \brief Reference type.
        using TReference = Mutable<TType>;
    };

    /// \brief Represents a span of read-write elements.
    template <typename TType>
    using RWSpan = BaseSpan<TType, RWSpanTypeTraits<TType>>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // BaseSpan.
    // =========

    /// \brief Get the number of elements in a span.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr Int Count(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept;

    /// \brief Access underlying span data.
    /// \remarks Accessing data of an empty span is allowed but the returned value is unspecified.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr typename TTraits::TPointer Data(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept;

    /// \brief Check whether lhs and rhs are equivalent.
    template <typename TType, typename TTraits, typename UType, typename UTraits>
    [[nodiscard]] constexpr Bool operator==(Immutable<BaseSpan<TType, TTraits>> lhs, Immutable<BaseSpan<UType, UTraits>> rhs) noexcept;

    /// \brief Compare two spans lexicographically.
    template <typename TType, typename TTraits, typename UType, typename UTraits>
    [[nodiscard]] constexpr Ordering operator<=>(Immutable<BaseSpan<TType, TTraits>> lhs, Immutable<BaseSpan<UType, UTraits>> rhs) noexcept;

    // Access.
    // =======

    /// \brief Convert rhs to a read-only span.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr Span<TType> ToReadOnly(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept;

    /// \brief Convert rhs to a read-write span.
    /// \remarks If the original span is not read-writable, accessing the returned values results in undefined behavior.
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr RWSpan<TType> ToReadWrite(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a new span by deducing template from arguments.
    template <typename TType>
    [[nodiscard]] constexpr Span<TType> MakeSpan(Ptr<TType> begin, Int count) noexcept;

    /// \brief Create a new span by deducing template from arguments.
    template <typename TType>
    [[nodiscard]] constexpr Span<TType> MakeSpan(Ptr<TType> begin, Ptr<TType> end) noexcept;

    /// \brief Create a new span by deducing template from arguments.
    template <typename TType>
    [[nodiscard]] constexpr RWSpan<TType> MakeSpan(RWPtr<TType> begin, Int count) noexcept;

    /// \brief Create a new span by deducing template from arguments.
    template <typename TType>
    [[nodiscard]] constexpr RWSpan<TType> MakeSpan(RWPtr<TType> begin, RWPtr<TType> end) noexcept;

}

// ===========================================================================

namespace Syntropy::Ranges::Templates
{
    /************************************************************************/
    /* RANGE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for spans.
    template <typename TType, typename TTraits>
    struct ElementReferenceTypeTraits<Ranges::BaseSpan<TType, TTraits>> : Syntropy::Templates::Alias<typename TTraits::TReference> {};

    /// \brief Specialization for spans.
    template <typename TType, typename TTraits>
    struct ElementPointerTypeTraits<Ranges::BaseSpan<TType, TTraits>> : Syntropy::Templates::Alias<typename TTraits::TPointer> {};

    /// \brief Specialization for spans.
    template <typename TType, typename TTraits>
    struct ElementCountTypeTraits<Ranges::BaseSpan<TType, TTraits>> : Syntropy::Templates::Alias<Int> {};
}

// ===========================================================================

namespace Syntropy::Ranges
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // BaseSpan<TType>.
    // ================

    template <typename TType, typename TTraits>
    constexpr BaseSpan<TType, TTraits>::BaseSpan(Null) noexcept
    {

    }

    template <typename TType, typename TTraits>
    constexpr BaseSpan<TType, TTraits>::BaseSpan(Immutable<BaseSpan<TType, TTraits>::TPointer> begin, Int count) noexcept
        : data_(begin)
        , count_(count)
    {

    }

    template <typename TType, typename TTraits>
    constexpr BaseSpan<TType, TTraits>::BaseSpan(Immutable<BaseSpan<TType, TTraits>::TPointer> begin, Immutable<BaseSpan<TType, TTraits>::TPointer> end) noexcept
        : BaseSpan(begin, ToInt(end - begin))
    {

    }

    template <typename TType, typename TTraits>
    template <typename UType, typename UTraits>
    constexpr BaseSpan<TType, TTraits>::BaseSpan(Immutable<BaseSpan<UType, UTraits>> rhs) noexcept
        : data_(ToPtr<TType>(Data(rhs)))
        , count_(Count(rhs))
    {

    }

    template <typename TType, typename TTraits>
    template <typename UType, typename UTraits>
    constexpr Mutable<BaseSpan<TType, TTraits>> BaseSpan<TType, TTraits>::operator=(Immutable<BaseSpan<UType, UTraits>> rhs) noexcept
    {
        data_ = ToPtr<TType>(Data(rhs));
        count_ = Count(rhs);

        return *this;
    }
    
    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr BaseSpan<TType, TTraits>::operator Bool() const noexcept
    {
        return count_ > ToInt(0);
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr typename BaseSpan<TType, TTraits>::TReference BaseSpan<TType, TTraits>::operator[](Int index) const noexcept
    {
        return data_[index];
    }

    // Non-member functions.
    // =====================

    // BaseSpan.

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr Int Count(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept
    {
        return rhs.count_;
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr typename TTraits::TPointer Data(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept
    {
        return rhs.data_;
    }

    template <typename TType, typename TTraits, typename UType, typename UTraits>
    [[nodiscard]] constexpr Bool operator==(Immutable<BaseSpan<TType, TTraits>> lhs, Immutable<BaseSpan<UType, UTraits>> rhs) noexcept
    {
        return AreEqual(lhs, rhs) && AreEquivalent(lhs, rhs);
    }

    template <typename TType, typename TTraits, typename UType, typename UTraits>
    [[nodiscard]] constexpr Ordering operator<=>(Immutable<BaseSpan<TType, TTraits>> lhs, Immutable<BaseSpan<UType, UTraits>> rhs) noexcept
    {
        return Compare(lhs, rhs);
    }

    // Conversion.

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr Span<TType> ToReadOnly(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept
    {
        return { ToReadOnly(Data(rhs)), Count(rhs) };
    }

    template <typename TType, typename TTraits>
    [[nodiscard]] constexpr RWSpan<TType> ToReadWrite(Immutable<BaseSpan<TType, TTraits>> rhs) noexcept
    {
        return { ToReadWrite(Data(rhs)), Count(rhs) };
    }

    // Utilities.

    template <typename TType>
    [[nodiscard]] constexpr Span<TType> MakeSpan(Ptr<TType> begin, Int count) noexcept
    {
        return { begin, count };
    }

    template <typename TType>
    [[nodiscard]] constexpr Span<TType> MakeSpan(Ptr<TType> begin, Ptr<TType> end) noexcept
    {
        return { begin, end };
    }

    template <typename TType>
    [[nodiscard]] constexpr RWSpan<TType> MakeSpan(RWPtr<TType> begin, Int count) noexcept
    {
        return { begin, count };
    }

    template <typename TType>
    [[nodiscard]] constexpr RWSpan<TType> MakeSpan(RWPtr<TType> begin, RWPtr<TType> end) noexcept
    {
        return { begin, end };
    }

}

// ===========================================================================