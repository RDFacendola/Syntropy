
// ################################################################################

// @brief Contains definition for unique pointers.
// @author Raffaele D. Facendola - April 2023

// ################################################################################

export module syntropy.unique;

import syntropy.types;
import syntropy.allocator;
import syntropy.memory;

// ################################################################################

export namespace sy
{
    // ================================================================================
    // UNIQUE
    // ================================================================================

    // A pointer that holds exclusive ownership of an object and disposes it when going
    // out of scope.
    template <typename Type>
    class Unique
    {

        template <typename TypeU, typename... Arguments>
        friend Unique<TypeU> MakeUnique(Arguments&&... arguments);

    public:

        // Default constructor.
        constexpr Unique() = default;

        // Create an empty pointer.
        constexpr Unique(Null);

        // No copy-constructor.
        constexpr Unique(const Unique&) = delete;

        // Move converting constructor.
        template <typename TypeU>
        constexpr Unique(Unique<TypeU>&& rhs);

        // Destroy the underlying object.
        constexpr ~Unique();

        // No copy-assignment.
        constexpr Unique& operator=(const Unique&) = delete;

        // Move assignment.
        template <typename TypeU>
        constexpr Unique& operator=(Unique<TypeU>&& rhs);

        // Reset.
        constexpr Unique& operator=(Null);

        // Check whether the pointer is valid.
        constexpr explicit operator Bool() const;

        // Access the underlying object.
        constexpr Type& operator*() const;

        // Access the underlying object.
        constexpr Type* operator->() const;

    private:

        // Create an empty pointer.
        constexpr Unique(RWAddress<Type> rhs);

        // Contains allocation infos about a memory block.
        struct Header;

        // Owned object.
        RWAddress<Type> _object{ nullptr };

    };

    // Contains allocation infos about a unique memory block.
    template <typename Type>
    struct Unique<Type>::Header
    {
        // Allocator the block was allocated on.
        RWAddress<Allocator> _allocator{ nullptr };

        // Size of the allocated block.
        Bytes _size{ 0 };
    };

    // Create a new object on the active allocator.
    // See: GetThreadAllocator().
    template <typename Type, typename... Arguments>
    Unique<Type> MakeUnique(Arguments&&... arguments);

}

// ################################################################################

// ================================================================================
// IMPLEMENTATION
// ================================================================================

template <typename Type>
constexpr sy::Unique<Type>::Unique(Null)
    : _object{ nullptr }
{

}

template <typename Type>
constexpr sy::Unique<Type>::Unique(RWAddress<Type> rhs)
    : _object{ rhs }
{

}

template <typename Type>
template <typename TypeU>
constexpr sy::Unique<Type>::Unique(Unique<TypeU>&& rhs)
    : _object{ rhs._object }
{
    rhs._object = nullptr;
}

template <typename Type>
constexpr sy::Unique<Type>::~Unique()
{
    *this = nullptr;
}

template <typename Type>
template <typename TypeU>
constexpr sy::Unique<Type>& sy::Unique<Type>::operator=(Unique<TypeU>&& rhs)
{
    *this = nullptr;

    _object = rhs._object;

    rhs._object = nullptr;

    return *this;
}

template <typename Type>
constexpr sy::Unique<Type>& sy::Unique<Type>::operator=(Null)
{
    if (_object)
    {
        // The header is allocated just before the payload.

        auto header_size = SizeOf<Header>();

        auto payload_block = Marshal(*_object);

        auto header_block = RWByteSpan{ payload_block.GetData() - header_size, ToInt(header_size) };

        auto& header = Unmarshal<Header>(header_block);

        auto block = RWByteSpan{ header_block.GetData(), ToInt(header._size) };

        header._allocator->Deallocate(block);

        _object = nullptr;
    }

    return *this;
}

template <typename Type>
constexpr sy::Unique<Type>::operator sy::Bool() const
{
    return !!_object;
}

template <typename Type>
constexpr Type& sy::Unique<Type>::operator*() const
{
    return *_object;
}

template <typename Type>
constexpr Type* sy::Unique<Type>::operator->() const
{
    return _object;
}

template <typename Type, typename... Arguments>
sy::Unique<Type> sy::MakeUnique(Arguments&&... arguments)
{
    using Header = typename sy::Unique<Type>::Header;

    auto header_size = SizeOf<Header>();
    auto type_size = SizeOf<Type>();
    auto block_size = header_size + type_size;

    auto& allocator = GetThreadAllocator();

    if (auto block = allocator.Allocate(block_size))
    {
        // Header.

        auto header_block = block.SelectFront(ToInt(header_size));

        auto& header = Unmarshal<Header>(header_block);

        header._allocator = AddressOf(allocator);
        header._size = block_size;

        // Payload.

        auto payload_block = block.SelectBack(ToInt(type_size));

        auto& payload = Unmarshal<Type>(payload_block);

        // Construct the object.

        new (AddressOf(payload)) Type(Forward<Arguments>(arguments)...);

        return sy::Unique<Type>{ AddressOf(payload) };
    }

    return nullptr;
}

// ################################################################################
