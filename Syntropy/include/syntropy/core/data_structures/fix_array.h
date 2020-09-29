
/// \file fix_array.h
/// \brief This header is part of the Syntropy core module. It contains definitions for fixed-size arrays.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/language/traits.h"
#include "syntropy/language/support/initializer_list.h"

#include "syntropy/core/foundation/span.h"

namespace Syntropy
{
    /************************************************************************/
    /* FIX ARRAY                                                            */
    /************************************************************************/

    /// \brief Represent a fixed-size contiguous sequence of elements.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename TElement, Int kRank>
    class FixArray
    {
        static_assert(Templates::IsReadWrite<TElement>, "TElement must be read-write.");

    public:

        /// \brief Access an element by index.
        /// If the provided index is not within the array the behavior of this method is undefined.
        constexpr Reference<TElement> operator[](Int index) const noexcept;

        /// \brief Access an element by index.
        /// If the provided index is not within the array the behavior of this method is undefined.
        constexpr RWReference<TElement> operator[](Int index) noexcept;

        /// \brief Access the underlying memory.
        constexpr Span<TElement> GetData() const noexcept;

        /// \brief Access the underlying memory.
        constexpr RWSpan<TElement> GetData() noexcept;

    // private:

        // Elements storage.
        TElement elements_[kRank];
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Iterators.

    /// \brief Get an iterator to the first element in a span.
    template <typename TElement, Int kRank>
    constexpr Pointer<TElement> begin(Reference<FixArray<TElement, kRank>> fix_array) noexcept;

    /// \brief Get an iterator past the last element in a span.
    template <typename TElement, Int kRank>
    constexpr Pointer<TElement> end(Reference<FixArray<TElement, kRank>> fix_array) noexcept;

    /// \brief Get an iterator to the first element in a span.
    template <typename TElement, Int kRank>
    constexpr RWPointer<TElement> begin(RWReference<FixArray<TElement, kRank>> fix_array) noexcept;

    /// \brief Get an iterator past the last element in a span.
    template <typename TElement, Int kRank>
    constexpr RWPointer<TElement> end(RWReference<FixArray<TElement, kRank>> fix_array) noexcept;

    // Modifiers.

    /// \brief Swap the content of both lhs and rhs.
    template <typename TElement, Int kRank>
    constexpr void Swap(RWReference<FixArray<TElement, kRank>> lhs, RWReference<FixArray<TElement, kRank>> rhs) noexcept;

    /// \brief Assign rhs to each element of lhs.
    template <typename TElement, Int kRank>
    constexpr void Fill(RWReference<FixArray<TElement, kRank>> lhs, Reference<TElement> rhs) noexcept;

    // Observers.

    /// \brief Check whether a fix-array is empty.
    /// \return Returns true if the fix-array is empty, returns false otherwise.
    template <typename TElement, Int kRank>
    constexpr Bool IsEmpty(Reference<FixArray<TElement, kRank>> fix_array) noexcept;

    /// \brief Get the number of elements in a fix-array.
    template <typename TElement, Int kRank>
    constexpr Int Count(Reference<FixArray<TElement, kRank>> fix_array) noexcept;

    // Comparison.

    /// \brief Check whether lhs and rhs are element-wise equivalent.
    template <typename TElement, typename UElement, Int kRank>
    constexpr Bool operator==(Reference<FixArray<TElement, kRank>> lhs, Reference<FixArray<UElement, kRank>> rhs) noexcept;

    /// \brief Check whether lhs and rhs are element-wise non-equivalent.
    template <typename TElement, typename UElement, Int kRank>
    constexpr Bool operator!=(Reference<FixArray<TElement, kRank>> lhs, Reference<FixArray<UElement, kRank>> rhs) noexcept;

    /// \brief Check whether lhs and rhs are element-wise equivalent.
    template <typename TElement, typename UElement, Int kRank>
    constexpr Bool Equals(Reference<FixArray<TElement, kRank>> lhs, Reference<FixArray<UElement, kRank>> rhs) noexcept;

    // Conversions.

    /// \brief Get a read-only span to a fix-array elements.
    template <typename TElement, Int kRank>
    constexpr Span<TElement> ToSpan(Reference<FixArray<TElement, kRank>> fix_array) noexcept;

    /// \brief Get a read-write span to a fix-array elements.
    template <typename TElement, Int kRank>
    constexpr RWSpan<TElement> ToRWSpan(RWReference<FixArray<TElement, kRank>> fix_array) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // FixArray<TElement, kRank>.
    // ==========================

    template <typename TElement, Int kRank>
    constexpr Reference<TElement> FixArray<TElement, kRank>::operator[](Int index) const noexcept
    {
        return elements_[index];
    }

    template <typename TElement, Int kRank>
    constexpr RWReference<TElement> FixArray<TElement, kRank>::operator[](Int index) noexcept
    {
        return elements_[index];
    }

    template <typename TElement, Int kRank>
    constexpr Span<TElement> FixArray<TElement, kRank>::GetData() const noexcept
    {
        return { elements_, kRank };
    }

    template <typename TElement, Int kRank>
    constexpr RWSpan<TElement> FixArray<TElement, kRank>::GetData() noexcept
    {
        return { elements_, kRank };
    }

    // Non-member functions.
    // =====================

    // Accessors.

    template <typename TElement, Int kRank>
    constexpr XPointer<TElement> begin(Reference<FixArray<TElement, kRank>> fix_array) noexcept
    {
        return Begin(fix_array.GetData());
    }

    template <typename TElement, Int kRank>
    constexpr XPointer<TElement> end(Reference<FixArray<TElement, kRank>> fix_array) noexcept
    {
        return End(fix_array.GetData());
    }

    template <typename TElement, Int kRank>
    constexpr RWPointer<TElement> begin(RWReference<FixArray<TElement, kRank>> fix_array) noexcept
    {
        return Begin(fix_array.GetData());
    }

    template <typename TElement, Int kRank>
    constexpr RWPointer<TElement> end(RWReference<FixArray<TElement, kRank>> fix_array) noexcept
    {
        return End(fix_array.GetData());
    }

    // Modifiers.

    template <typename TElement, Int kRank>
    constexpr void Swap(RWReference<FixArray<TElement, kRank>> lhs, RWReference<FixArray<TElement, kRank>> rhs) noexcept
    {
        // #TODO Optimize for trivially swappable elements.

        for (auto index = 0; index < kRank; ++index)
        {
            Swap(lhs[index], rhs[index]);
        }
    }

    template <typename TElement, Int kRank>
    constexpr void Fill(RWReference<FixArray<TElement, kRank>> lhs, Reference<TElement> rhs) noexcept
    {
        for (auto&& element : lhs)
        {
            element = rhs;
        }
    }

    // Observers.

    template <typename TElement, Int kRank>
    constexpr Bool IsEmpty(Reference<FixArray<TElement, kRank>> fix_array) noexcept
    {
        return Count(fix_array) == 0;
    }

    template <typename TElement, Int kRank>
    constexpr Int Count(Reference<FixArray<TElement, kRank>> fix_array) noexcept
    {
        return kRank;
    }

    // Comparison.

    template <typename TElement, typename UElement, Int kRank>
    constexpr Bool operator==(Reference<FixArray<TElement, kRank>> lhs, Reference<FixArray<UElement, kRank>> rhs) noexcept
    {
        return Equals(lhs, rhs);
    }

    template <typename TElement, typename UElement, Int kRank>
    constexpr Bool operator!=(Reference<FixArray<TElement, kRank>> lhs, Reference<FixArray<UElement, kRank>> rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template <typename TElement, typename UElement, Int kRank>
    constexpr Bool Equals(Reference<FixArray<TElement, kRank>> lhs, Reference<FixArray<UElement, kRank>> rhs) noexcept
    {
        return Equals(lhs.GetData(), rhs.GetData());
    }

    // Conversions.

    template <typename TElement, Int kRank>
    constexpr Span<TElement> ToSpan(Reference<FixArray<TElement, kRank>> fix_array) noexcept
    {
        return fix_array.GetData();
    }

    template <typename TElement, Int kRank>
    constexpr RWSpan<TElement> ToRWSpan(RWReference<FixArray<TElement, kRank>> fix_array) noexcept
    {
        return ReadWrite(fix_array.GetData());
    }

}
