
// ################################################################################

/// @brief Contains span definitions.
/// @author Raffaele D. Facendola - April 2023

// ################################################################################

module;

#include <cstdint>
#include <cstddef>

// ################################################################################

export module syntropy.span;

import syntropy.types;

// ################################################################################

export namespace sy
{
    // ================================================================================
    // BASE SPAN
    // ================================================================================

    // A contiguous, non-owning range of elements.
    template <typename Traits>
    class BaseSpan
    {

        template <typename TraitsU>
        friend class BaseSpan;

        template <typename TraitsU>
        friend constexpr typename TraitsU::AddressType begin(const BaseSpan<TraitsU>& rhs);

        template <typename TraitsU>
        friend constexpr typename TraitsU::AddressType end(const BaseSpan<TraitsU>& rhs);

    public:

        // Element type.
        using ElementType = typename Traits::ElementType;

        // Address type.
        using AddressType = typename Traits::AddressType;

        // Reference type.
        using ReferenceType = typename Traits::ReferenceType;

    public:

        // Create an empty span.
        constexpr BaseSpan() = default;

        // Create an empty span.
        constexpr BaseSpan(Null);

        // Create a span given the first element and its count.
        constexpr BaseSpan(AddressType begin, Int count);

        // Create a span given the first element and one past the last one.
        constexpr BaseSpan(AddressType begin, AddressType end);

        // Converting copy-constructor.
        template <typename TraitsU>
        constexpr BaseSpan(const BaseSpan<TraitsU>& rhs);

        // Default destructor.
        ~BaseSpan() = default;

        // Copy-assignment operator.
        template <typename TraitsU>
        constexpr BaseSpan& operator=(const BaseSpan<TraitsU>& rhs);

        // Access a span element by index.
        // Accessing an element past the span range results in undefined behaviour.
        constexpr ReferenceType operator[](Int index) const;

        // Get the address to the first element in the span.
        constexpr AddressType GetData() const;

        // Get the number of elements in the span.
        constexpr Int GetCount() const;

        // Check whether the span is empty.
        constexpr Bool IsEmpty() const;

        // Access the first element in the span.
        // Accessing the returned value when the span is empty results in undefined behaviour.
        constexpr ReferenceType GetFront() const;

        // Access the last element in the span.
        // Accessing the returned value when the span is empty results in undefined behaviour.
        constexpr ReferenceType GetBack() const;
        
        // Select a sub-range of elements in the span.
        // Exceeding span boundaries results in undefined behaviour.
        constexpr BaseSpan Select(Int offset, Int count) const;

        // Select a sub-range of elements in the span starting from the front.
        // Exceeding span boundaries results in undefined behaviour.
        constexpr BaseSpan SelectFront(Int count) const;

        // Select a sub-range of elements in the span starting from the back.
        // Exceeding span boundaries results in undefined behaviour.
        constexpr BaseSpan SelectBack(Int count) const;

    private:

        // Address to the first element in the span.
        AddressType _data{ nullptr };

        // Number of elements in the span.
        Int _count{};

    };

    // Type-traits for a base span.
    template <typename ElementTypeT, typename AddressTypeT, typename ReferenceTypeT>
    struct BaseSpanTraits
    {
        // Element type.
        using ElementType = ElementTypeT;

        // Address type.
        using AddressType = AddressTypeT;

        // Reference type.
        using ReferenceType = ReferenceTypeT;
    };

    // ================================================================================
    // SPAN / RW-SPAN
    // ================================================================================

    // A read-only span of elements.
    template <typename ElementType>
    using Span = BaseSpan<BaseSpanTraits<ElementType, Address<ElementType>, const ElementType&>>;

    // A read-write span of elements.
    template <typename ElementType>
    using RWSpan = BaseSpan<BaseSpanTraits<ElementType, RWAddress<ElementType>, ElementType&>>;

    // ================================================================================
    // DEDUCTION GUIDES
    // ================================================================================

    template<class ElementType>
    BaseSpan(Address<ElementType>, Int) -> BaseSpan<BaseSpanTraits<ElementType, Address<ElementType>, const ElementType&>>;

    template<class ElementType>
    BaseSpan(RWAddress<ElementType>, Int) -> BaseSpan<BaseSpanTraits<ElementType, RWAddress<ElementType>, ElementType&>>;

    // ================================================================================
    // ACCESS
    // ================================================================================

    // Convert a span to read-only.
    template <typename Traits>
    constexpr Span<typename Traits::ElementType> ToReadOnly(const BaseSpan<Traits>& rhs);

    // Convert a span to read-write.
    // If the underlying span is not read-writable, accessing the returned value results in undefined behaviour.
    template <typename Traits>
    constexpr RWSpan<typename Traits::ElementType> ToReadWrite(const BaseSpan<Traits>& rhs);

    // ================================================================================
    // HELPERS
    // ================================================================================

    // Create a read-only span by deducing template from arguments.
    template <typename Type>
    constexpr Span<Type> MakeSpan(Address<Type> begin, Int count);

    // Create a read-only span by deducing template from arguments.
    template <typename Type>
    constexpr Span<Type> MakeSpan(Address<Type> begin, Address<Type> end);

    // Create a read-write span by deducing template from arguments.
    template <typename Type>
    constexpr RWSpan<Type> MakeSpan(RWAddress<Type> begin, Int count);

    // Create a read-write span by deducing template from arguments.
    template <typename Type>
    constexpr RWSpan<Type> MakeSpan(RWAddress<Type> begin, RWAddress<Type> end);

    // Create a span from a C array.
    template <typename Type, Int Count>
    constexpr auto MakeSpan(Type(&rhs)[Count]);

    // ================================================================================
    // ITERATORS
    // ================================================================================

    // Get an iterator to the first element in a span.
    template <typename Traits>
    constexpr typename Traits::AddressType begin(const BaseSpan<Traits>& rhs);

    // Get an iterator past the last element in a span.
    template <typename Traits>
    constexpr typename Traits::AddressType end(const BaseSpan<Traits>& rhs);

}

// ################################################################################

// ================================================================================
// BASE SPAN
// ================================================================================

template <typename Traits>
constexpr sy::BaseSpan<Traits>::BaseSpan(Null)
{

}

template <typename Traits>
constexpr sy::BaseSpan<Traits>::BaseSpan(AddressType begin, Int count)
    : _data{ begin }
    , _count{ count }
{

}

template <typename Traits>
constexpr sy::BaseSpan<Traits>::BaseSpan(AddressType begin, AddressType end)
    : _data{ begin }
    , _count{ Int{ end - begin } }
{

}

template <typename Traits>
template <typename TraitsU>
constexpr sy::BaseSpan<Traits>::BaseSpan(const BaseSpan<TraitsU>& rhs)
    : _data{ static_cast<AddressType>(rhs._data) }
    , _count{ rhs._count }
{

}

template <typename Traits>
template <typename TraitsU>
constexpr sy::BaseSpan<Traits>& sy::BaseSpan<Traits>::operator=(const BaseSpan<TraitsU>& rhs)
{
    _data = static_cast<AddressType>(rhs._data);
    _count = rhs._count;

    return *this;
}

template <typename Traits>
constexpr sy::BaseSpan<Traits>::ReferenceType sy::BaseSpan<Traits>::operator[](Int index) const
{
    return _data[index];
}

template <typename Traits>
constexpr sy::BaseSpan<Traits>::AddressType sy::BaseSpan<Traits>::GetData() const
{
    return _data;
}

template <typename Traits>
constexpr sy::Int sy::BaseSpan<Traits>::GetCount() const
{
    return _count;
}

template <typename Traits>
constexpr sy::Bool sy::BaseSpan<Traits>::IsEmpty() const
{
    return (_count == 0);
}

template <typename Traits>
constexpr sy::BaseSpan<Traits>::ReferenceType sy::BaseSpan<Traits>::GetFront() const
{
    return *_data;
}

template <typename Traits>
constexpr sy::BaseSpan<Traits>::ReferenceType sy::BaseSpan<Traits>::GetBack() const
{
    return *(_data + _count - 1);
}

template <typename Traits>
constexpr sy::BaseSpan<Traits> sy::BaseSpan<Traits>::Select(Int offset, Int count) const
{
    return BaseSpan{ _data + offset, count };
}

template <typename Traits>
constexpr sy::BaseSpan<Traits> sy::BaseSpan<Traits>::SelectFront(Int count) const
{
    return BaseSpan{ _data, count };
}

template <typename Traits>
constexpr sy::BaseSpan<Traits> sy::BaseSpan<Traits>::SelectBack(Int count) const
{
    return BaseSpan{ _data + _count - count, count };
}

// ================================================================================
// ACCESS
// ================================================================================

template <typename Traits>
constexpr sy::Span<typename Traits::ElementType> sy::ToReadOnly(const BaseSpan<Traits>& rhs)
{
    return { ToReadOnly(rhs.GetData()), rhs.GetCount() };
}

template <typename Traits>
constexpr sy::RWSpan<typename Traits::ElementType> sy::ToReadWrite(const BaseSpan<Traits>& rhs)
{
    return { ToReadWrite(rhs.GetData()), rhs.GetCount() };
}

// ================================================================================
// HELPERS
// ================================================================================

template <typename Type>
constexpr sy::Span<Type> sy::MakeSpan(Address<Type> begin, Int count)
{
    return { begin, count };
}

template <typename Type>
constexpr sy::Span<Type> sy::MakeSpan(Address<Type> begin, Address<Type> end)
{
    return { begin, end };
}

template <typename Type>
constexpr sy::RWSpan<Type> sy::MakeSpan(RWAddress<Type> begin, Int count)
{
    return { begin, count };
}

template <typename Type>
constexpr sy::RWSpan<Type> sy::MakeSpan(RWAddress<Type> begin, RWAddress<Type> end)
{
    return { begin, end };
}

template <typename Type, sy::Int Count>
constexpr auto sy::MakeSpan(Type(&rhs)[Count])
{
    return MakeSpan(rhs, Count);
}

// ================================================================================
// ITERATORS
// ================================================================================

template <typename Traits>
constexpr typename Traits::AddressType sy::begin(const BaseSpan<Traits>& rhs)
{
    return rhs._data;
}

template <typename Traits>
constexpr typename Traits::AddressType sy::end(const BaseSpan<Traits>& rhs)
{
    return rhs._data + rhs._count;
}

// ################################################################################
