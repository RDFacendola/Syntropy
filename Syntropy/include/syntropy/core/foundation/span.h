
/// \file span.h
/// \brief This header is part of Syntropy core module. It contains definition of spans.
///
/// \author Raffaele D. Facendola - Jun 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/templates/type_traits.h"

#include "syntropy/core/foundation/tuple.h"
#include "syntropy/core/foundation/range.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* SPAN                                                                 */
    /************************************************************************/

    /// \brief Represents a contiguous, non-owning, range of elements.
    /// \author Raffaele D. Facendola - June 2020.
    template <typename TType>
    class Span
    {

    public:

        /// \brief Create an empty span.
        constexpr Span() noexcept = default;

        /// \brief Create an empty span.
        constexpr Span(Null) noexcept;

        /// \brief Create a span given a pointer to the first element and the number of elements.
        template <typename TBegin>
        constexpr Span(TBegin begin, Int count) noexcept;

        /// \brief Create a span given a pointer to the first element and a pointer past the last element.
        template <typename TBegin, typename TEnd, typename = Templates::EnableIf<!Templates::IsConvertible<TEnd, Int>>>
        constexpr Span(TBegin begin, TEnd end) noexcept;

        /// \brief Copy constructor.
        template <typename UType>
        constexpr Span(Immutable<Span<UType>> rhs) noexcept;

        /// \brief Default destructor.
        ~Span() noexcept = default;

        /// \brief Copy assignment operator.
        template <typename UType>
        constexpr Mutable<Span> operator=(Immutable<Span<UType>> rhs) noexcept;

        /// \brief Check whether the span is non-empty.
        constexpr operator Bool() const noexcept;

        /// \brief Access an element by index.
        /// If the provided index is not within the span the behavior of this method is undefined.
        constexpr Reference<TType> operator[](Int index) const noexcept;

        /// \brief Get the number of elements in the span.
        constexpr Int GetCount() const noexcept;

        /// \brief Access the underlying memory.
        constexpr Pointer<TType> GetData() const noexcept;

        /// \brief Swap this span with rhs.
        constexpr void Swap(Mutable<Span> rhs) noexcept;

    private:

        /// \brief Pointer to the first element in the range.
        Pointer<TType> data_{ nullptr };

        /// \brief Number of elements in the span.
        Int count_{ 0 };

    };

    /************************************************************************/
    /* TYPE ALIASES                                                         */
    /************************************************************************/

    /// \brief Alias for a common type between two or more spans.
    template <typename... TTypes>
    using CommonSpan = Span<Templates::RemovePointer<Templates::CommonType<Templates::AddPointer<TTypes>...>>>;

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Forward range.
    // ==============

    /// \brief Access the first element in a span.
    /// \remarks Accessing the first element of an empty span results in undefined behavior.
    template <typename TType>
    constexpr Reference<TType> Front(Immutable<Span<TType>> rhs) noexcept;

    /// \brief Discard the first count elements in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    constexpr Span<TType> PopFront(Immutable<Span<TType>> rhs) noexcept;

    /// \brief Check whether a span is empty.
    /// \return Returns true if the span is empty, returns false otherwise.
    template <typename TType>
    constexpr Bool IsEmpty(Immutable<Span<TType>> rhs) noexcept;

    // Bidirectional range.
    // ====================

    /// \brief Access the last element in a span.
    /// \remarks Accessing the last element of an empty span results in undefined behavior.
    template <typename TType>
    constexpr Reference<TType> Back(Immutable<Span<TType>> rhs) noexcept;

    /// \brief Discard the last count elements in a span and return the resulting subspan.
    /// \remarks If this method would cause the subspan to exceed the original span, the behavior of this method is undefined.
    template <typename TType>
    constexpr Span<TType> PopBack(Immutable<Span<TType>> rhs) noexcept;

    // Random access range.
    // ====================

    /// \brief Get the number of elements in a span.
    template <typename TType>
    constexpr Int Count(Immutable<Span<TType>> rhs) noexcept;

    /// \brief Obtain a sub-span given an offset and a number of elements.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    template <typename TType>
    constexpr Span<TType> Select(Immutable<Span<TType>> rhs, Int offset, Int count) noexcept;

    /// \brief Obtain a span element at given index.
    /// \remarks Exceeding span boundaries results in undefined behavior.
    template <typename TType>
    constexpr Reference<TType> Select(Immutable<Span<TType>> rhs, Int index) noexcept;

    // Contiguous range.
    // =================

    /// \brief Access underlying span data.
    /// \remarks Accessing data of an empty span is allowed but the returned value is unspecified.
    template <typename TType>
    constexpr Pointer<TType> Data(Reference<Span<TType>> rhs) noexcept;

    // Comparisons.
    // ============

    /// \brief Check whether lhs and rhs are identical.
    template <typename TType, typename UType>
    constexpr Bool operator==(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Create a new span by deducing template from arguments.
    template <typename TBegin>
    constexpr Span<TBegin> MakeSpan(Pointer<TBegin> begin, Int count) noexcept;

    /// \brief Create a new span by deducing template from arguments.
    template <typename TBegin, typename TEnd>
    constexpr Templates::CommonType<TBegin, TEnd> MakeSpan(Pointer<TBegin> begin, Pointer<TEnd> end) noexcept;

    /// \brief Swap two spans
    template <typename TType>
    constexpr void Swap(Mutable<Span<TType>> lhs, Mutable<Span<TType>> rhs) noexcept;

}

// ===========================================================================

namespace Syntropy::Templates
{
    /************************************************************************/
    /* RANGE TRAITS                                                         */
    /************************************************************************/

    /// \brief Specialization for spans.
    template <typename TType>
    struct RangeTraits<Span<TType>>
    {
        /// \brief Type of a reference to a range element.
        using ElementReferenceType = Reference<TType>;
    };
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Span<TType>.
    // ===============

    template <typename TType>
    constexpr Span<TType>::Span(Null) noexcept
    {

    }

    template <typename TType>
    template <typename TBegin>
    constexpr Span<TType>::Span(TBegin begin, Int count) noexcept
        : data_((count > 0) ? begin : nullptr)
        , count_(count)
    {

    }

    template <typename TType>
    template <typename TBegin, typename TEnd, typename>
    constexpr Span<TType>::Span(TBegin begin, TEnd end) noexcept
        : Span(begin, ToInt(end - begin))
    {

    }

    template <typename TType>
    template <typename UType>
    constexpr Span<TType>::Span(Immutable<Span<UType>> rhs) noexcept
        : data_(ToPtr<TType>(rhs.GetData()))
        , count_(rhs.GetCount())
    {

    }

    template <typename TType>
    template <typename UType>
    constexpr Mutable<Span<TType>> Span<TType>::operator=(Immutable<Span<UType>> rhs) noexcept
    {
        data_ = ToPtr<TType>(rhs.GetData());
        count_ = rhs.GetCount();

        return *this;
    }

    template <typename TType>
    constexpr Span<TType>::operator Bool() const noexcept
    {
        return count_ > 0;
    }

    template <typename TType>
    constexpr Reference<TType> Span<TType>::operator[](Int index) const noexcept
    {
        return data_[index];
    }

    template <typename TType>
    constexpr Int Span<TType>::GetCount() const noexcept
    {
        return count_;
    }

    template <typename TType>
    constexpr Pointer<TType> Span<TType>::GetData() const noexcept
    {
        return data_;
    }

    template <typename TType>
    constexpr void Span<TType>::Swap(Mutable<Span<TType>> rhs) noexcept
    {
        Syntropy::Swap(data_, rhs.data_);
        Syntropy::Swap(count_, rhs.count_);
    }

    // Non-member functions.
    // =====================

    // Forward Range.

    template <typename TType>
    constexpr Reference<TType> Front(Immutable<Span<TType>> rhs) noexcept
    {
        return *Data(rhs);
    }

    template <typename TType>
    constexpr Span<TType> PopFront(Immutable<Span<TType>> rhs) noexcept
    {
        return PopFront(rhs, 1);
    }

    template <typename TType>
    constexpr Bool IsEmpty(Immutable<Span<TType>> rhs) noexcept
    {
        return !rhs;
    }

    // Bidirectional range.

    template <typename TType>
    constexpr Reference<TType> Back(Immutable<Span<TType>> rhs) noexcept
    {
        return *(Data(rhs) + Count(rhs) - 1);
    }

    template <typename TType>
    constexpr Span<TType> PopBack(Immutable<Span<TType>> rhs) noexcept
    {
        return PopBack(rhs, 1);
    }

    // Random access range.

    template <typename TType>
    constexpr Int Count(Immutable<Span<TType>> rhs) noexcept
    {
        return rhs.GetCount();
    }

    template <typename TType>
    constexpr Span<TType> Select(Immutable<Span<TType>> rhs, Int offset, Int count) noexcept
    {
        return { Data(rhs) + offset, count };
    }

    template <typename TType>
    constexpr Reference<TType> Select(Immutable<Span<TType>> rhs, Int index) noexcept
    {
        return rhs[index];
    }

    // Contiguous range.

    template <typename TType>
    constexpr Pointer<TType> Data(Reference<Span<TType>> rhs) noexcept
    {
        return rhs.GetData();
    }


    // Comparisons.

    template <typename TType, typename UType>
    constexpr Bool operator==(Immutable<Span<TType>> lhs, Immutable<Span<UType>> rhs) noexcept
    {
        return (lhs.GetData() == rhs.GetData()) && (lhs.GetCount() == rhs.GetCount());
    }

    // Utilities.

    template <typename TBegin>
    constexpr Span<TBegin> MakeSpan(Pointer<TBegin> begin, Int count) noexcept
    {
        return { begin, count };
    }

    template <typename TBegin, typename TEnd>
    constexpr Templates::CommonType<TBegin, TEnd> MakeSpan(Pointer<TBegin> begin, Pointer<TEnd> end) noexcept
    {
        return { begin, end };
    }

    template <typename TType>
    constexpr void Swap(Mutable<Span<TType>> lhs, Mutable<Span<TType>> rhs) noexcept
    {
        lhs.Swap(rhs);
    }

}

// ===========================================================================