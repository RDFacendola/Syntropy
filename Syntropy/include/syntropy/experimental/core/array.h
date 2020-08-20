
/// \file array.h
/// \brief This header is part of the Syntropy core module. It contains array definitions.
/// 
/// In order to avoid ambiguities with vectors in mathematical sense, vectors in Syntropy are referred to "arrays".
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include <array>

#include "syntropy/language/language_types.h"
#include "syntropy/language/initializer_list.h"
#include "syntropy/diagnostics/assert.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/buffer.h"
#include "syntropy/allocators/allocator.h"
#include "syntropy/core/span.h"

namespace Syntropy
{
    /************************************************************************/
    /* ARRAY <ELEMENT>                                                      */
    /************************************************************************/

    /// \brief Represents a dynamically-sized contiguous sequence of elements.
    /// \author Raffaele D. Facendola - August 2020.
    template <typename TElement>
    class Array
    {
    public:

        /// \brief Construct an empty array bound to the active allocator.
        constexpr Array() noexcept;

        /// \brief Construct an empty array bound to an explicit allocator.
        constexpr explicit Array(Allocator& allocator) noexcept;

        /// \brief Copy constructor.
        constexpr Array(const Array& rhs) noexcept;

        /// \brief Copy constructor with explicit allocator.
        constexpr Array(const Array& rhs, Allocator& allocator = Memory::GetAllocator()) noexcept;

        /// \brief Move constructor.
        constexpr Array(Array&& rhs) noexcept;

        /// \brief Move constructor with explicit allocator.
        constexpr Array(Array&& rhs, Allocator& allocator) noexcept;

        /// \brief Construct an array with count default-initialized elements.
        constexpr explicit Array(Int count, Allocator& allocator = Memory::GetAllocator()) noexcept;

        /// \brief Construct an array with count copies of element.
        constexpr Array(Int count, const TElement& element, Allocator& allocator = Memory::GetAllocator()) noexcept;

        /// \brief Construct an array with the content of an initializer list.
        constexpr Array(InitializerList<TElement> elements, Allocator& allocator = Memory::GetAllocator()) noexcept;

        /// \brief Construct an array from a span of elements after conversion, if necessary.
        template <typename UElement>
        constexpr Array(const SpanT<UElement>& elements, Allocator& allocator = Memory::GetAllocator()) noexcept;

        /// \brief Default destructor.
        /*constexpr*/ ~Array() = default;

        /// \brief Copy-assignment operator.
        /// \remarks Rhs's allocator is not propagated.
        constexpr Array& operator=(const Array& rhs) noexcept;

        /// \brief Move-assignment operator.
        /// /// \remarks Rhs's allocator is not propagated.
        constexpr Array& operator=(Array&& rhs) noexcept;

        /// \brief Replace the content of the array with elements.
        constexpr Array& operator=(InitializerList<TElement> elements) noexcept;

        /// \brief Replace the content of the array with elements.
        constexpr Array& operator=(const SpanT<TElement>& elements) noexcept;

        /// \brief Access an element in the array by index.
        /// If the provided index is outside array bounds, the behavior of this method is undefined.
        constexpr TElement& operator[](Int index) noexcept;

        /// \brief Access an element in the array by index.
        /// If the provided index is outside array bounds, the behavior of this method is undefined.
        constexpr const TElement& operator[](Int index) const noexcept;

        /// \brief Access element storage.
        constexpr SpanT<TElement> GetData() noexcept;

        /// \brief Access element storage.
        constexpr Span<TElement> GetData() const noexcept;

        /// \brief Change the number of elements in the array.
        /// If the size of the array increases, additional elements are default-initialized.
        constexpr void Resize(Int count) noexcept;

        /// \brief Change the number of elements in the array
        /// If the size of the array increases, additional elements are copies of element.
        constexpr void Resize(Int count, const TElement& element);

        /// \brief Get the maximum number of elements the array can hold before reallocating.
        constexpr Int GetCapacity() const noexcept;

        /// \brief Grow the underlying memory storage up to count.
        /// If the provided number is smaller than the current capacity, this method behaves as no-op.
        constexpr void Reserve(Int count) noexcept;

        /// \brief Shrink the underlying memory storage to fit exactly the number of elements in the array.
        constexpr void Shrink() noexcept;

        /// \brief Construct an element at the end of the array in-place.
        /// If the new element would cause the array to grow past its capacity, this method causes the reallocation of the underlying storage.
        template <typename... TArguments>
        constexpr TElement& PushBack(TArguments&&... arguments) noexcept;

        /// \brief Remove an element on the end of the array.
        /// If the array is empty, the behavior of this method is undefined.
        constexpr void PopBack() noexcept;

        /// \brief Insert an element before position by constructing it in-place from arguments.
        /// \return Returns an iterator to the inserted element.
        template <typename... TArguments>
        constexpr PointerT<TElement> Insert(Pointer<TElement> position, TArguments&&... arguments) noexcept;

        /// \brief Insert count copies of element before position.
        /// \return Returns a span to the inserted elements.
        template <typename... TArguments>
        constexpr SpanT<TElement> Insert(Pointer<TElement> position, Int count, const TElement& element) noexcept;

        /// \brief Insert elements before position.
        /// \return Returns a span to the inserted elements.
        template <typename... TArguments>
        constexpr SpanT<TElement> Insert(Pointer<TElement> position, InitializerList<TElement> elements) noexcept;

        /// \brief Insert elements before position.
        /// \return Returns a span to the inserted elements.
        template <typename... TArguments>
        constexpr SpanT<TElement> Insert(Pointer<TElement> position, const SpanT<TElement>& elements) noexcept;

        /// \brief Erase all elements in a given span.
        /// \return Returns the span following the last removed element to the end of the vector.
        /// \remarks If span doesn't belong to this vector, the behavior of this method is undefined.
        constexpr Span<TElement> Erase(const Span<TElement>& span) noexcept;

        /// \brief Swap this array with rhs.
        /// If the current allocator is different than rhs's allocator, the behavior of this method is undefined.
        constexpr void Swap(Array<TElement>& rhs);

        /// \brief Get the array allocator.
        constexpr Allocator& GetAllocator() const;

    private:

        /// \brief Storage for array elements.
        Buffer storage_;

        /// \brief Elements span, relative to storage.
        SpanT<TElement> elements_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Iterators.

    /// \brief Get an iterator to the first element in a span.
    template <typename TElement>
    constexpr  Pointer<TElement> begin(Reference<Array<TElement>> span) noexcept;

    /// \brief Get an iterator past the last element in a span.
    template <typename TElement>
    constexpr  Pointer<TElement> end(Reference<Array<TElement>> span) noexcept;

    /// \brief Get an iterator to the first element in a span.
    template <typename TElement>
    constexpr  RWPointer<TElement> begin(RWReference<Array<TElement>> span) noexcept;

    /// \brief Get an iterator past the last element in a span.
    template <typename TElement>
    constexpr  RWPointer<TElement> end(RWReference<Array<TElement>> span) noexcept;

    // Observers.

    /// \brief Check whether an array is empty.
    /// \return Returns true if the array is empty, returns false otherwise.
    template <typename TElement>
    constexpr Bool IsEmpty(const Array<TElement>& array) noexcept;

    /// \brief Get the number of elements in a span.
    template <typename TElement>
    constexpr Int Count(const Array<TElement>& array) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Array<TElement>.
    // ================

    template <typename TElement>
    constexpr Array<TElement>::Array() noexcept
        : Array(Memory::GetAllocator())
    {

    }

    template <typename TElement>
    constexpr Array<TElement>::Array(Allocator& allocator) noexcept
        : storage_(allocator)
    {

    }

    template <typename TElement>
    constexpr Array<TElement>::Array(const Array& rhs) noexcept
        : Array(rhs, Memory::GetAllocator())
    {

    }

    template <typename TElement>
    constexpr Array<TElement>::Array(const Array& rhs, Allocator& allocator) noexcept
        : storage_(rhs.storage_)
    {
        // #TODO Propagate allocator?
    }

    template <typename TElement>
    constexpr Array<TElement>::Array(Array&& rhs) noexcept
        : Array(std::move(rhs), Memory::GetAllocator())
    {

    }

    template <typename TElement>
    constexpr Array<TElement>::Array(Array&& rhs, Allocator& allocator) noexcept
    {
        // #TODO Propagate allocator?
    }

    template <typename TElement>
    constexpr Array<TElement>::Array(Int count, Allocator& allocator) noexcept
    {

    }

    template <typename TElement>
    constexpr Array<TElement>::Array(Int count, const TElement& element, Allocator& allocator) noexcept
    {

    }

    template <typename TElement>
    constexpr Array<TElement>::Array(InitializerList<TElement> elements, Allocator& allocator) noexcept
    {

    }

    template <typename TElement>
    template <typename UElement>
    constexpr Array<TElement>::Array(const SpanT<UElement>& elements, Allocator& allocator) noexcept
    {

    }

    template <typename TElement>
    constexpr Array<TElement>& Array<TElement>::operator=(const Array& rhs) noexcept
    {

    }

    template <typename TElement>
    constexpr Array<TElement>& Array<TElement>::operator=(Array&& rhs) noexcept
    {
        rhs.Swap(*this);

        return *this;
    }

    template <typename TElement>
    constexpr Array<TElement>& Array<TElement>::operator=(InitializerList<TElement> elements) noexcept
    {
        return *this;
    }

    template <typename TElement>
    constexpr Array<TElement>& Array<TElement>::operator=(const SpanT<TElement>& elements) noexcept
    {
        return *this;
    }

    template <typename TElement>
    constexpr TElement& Array<TElement>::operator[](Int index) noexcept
    {
        return elements_[index];
    }

    template <typename TElement>
    constexpr const TElement& Array<TElement>::operator[](Int index) const noexcept
    {
        return elements_[index];
    }

    template <typename TElement>
    constexpr SpanT<TElement> Array<TElement>::GetData() noexcept
    {
        return elements_;
    }

    template <typename TElement>
    constexpr Span<TElement> Array<TElement>::GetData() const noexcept
    {
        return elements_;
    }

    template <typename TElement>
    constexpr void Array<TElement>::Resize(Int count) noexcept
    {

    }

    template <typename TElement>
    constexpr void Array<TElement>::Resize(Int count, const TElement& element)
    {

    }

    template <typename TElement>
    constexpr Int Array<TElement>::GetCapacity() const noexcept
    {
        return Memory::Size(storage_) / Memory::SizeOf<TElement>();
    }

    template <typename TElement>
    constexpr void Array<TElement>::Reserve(Int count) noexcept
    {

    }

    template <typename TElement>
    constexpr void Array<TElement>::Shrink() noexcept
    {

    }

    template <typename TElement>
    template <typename... TArguments>
    constexpr TElement& Array<TElement>::PushBack(TArguments&&... arguments) noexcept
    {

    }

    template <typename TElement>
    constexpr void Array<TElement>::PopBack() noexcept
    {

    }

    template <typename TElement>
    template <typename... TArguments>
    constexpr PointerT<TElement> Array<TElement>::Insert(Pointer<TElement> position, TArguments&&... arguments) noexcept
    {
        return {};
    }

    template <typename TElement>
    template <typename... TArguments>
    constexpr SpanT<TElement> Array<TElement>::Insert(Pointer<TElement> position, Int count, const TElement& element) noexcept
    {
        return {};
    }

    template <typename TElement>
    template <typename... TArguments>
    constexpr SpanT<TElement> Array<TElement>::Insert(Pointer<TElement> position, InitializerList<TElement> elements) noexcept
    {
        return {};
    }

    template <typename TElement>
    template <typename... TArguments>
    constexpr SpanT<TElement> Array<TElement>::Insert(Pointer<TElement> position, const SpanT<TElement>& elements) noexcept
    {
        return {};
    }

    template <typename TElement>
    constexpr Span<TElement> Array<TElement>::Erase(const Span<TElement>& span) noexcept
    {
        return {};
    }

    template <typename TElement>
    constexpr void Array<TElement>::Swap(Array<TElement>& rhs)
    {

    }

    template <typename TElement>
    constexpr Allocator& Array<TElement>::GetAllocator() const
    {
        return elements_.GetAllocator();
    }

    // Non-member functions.
    // =====================

    // Iterators.

    template <typename TElement>
    constexpr  Pointer<TElement> begin(Reference<Array<TElement>> span) noexcept
    {
        return Begin(span.GetData());
    }

    template <typename TElement>
    constexpr  Pointer<TElement> end(Reference<Array<TElement>> span) noexcept
    {
        return End(span.GetData());
    }

    template <typename TElement>
    constexpr  RWPointer<TElement> begin(RWReference<Array<TElement>> span) noexcept
    {
        return Begin(span.GetData());
    }

    template <typename TElement>
    constexpr  RWPointer<TElement> end(RWReference<Array<TElement>> span) noexcept
    {
        return End(span.GetData());
    }

    // Observers.

    template <typename TElement>
    constexpr Bool IsEmpty(const Array<TElement>& array) noexcept
    {
        return Count(array) == 0;
    }

    template <typename TElement>
    constexpr Int Count(const Array<TElement>& array) noexcept
    {
        return Count(array.GetData());
    }

}
