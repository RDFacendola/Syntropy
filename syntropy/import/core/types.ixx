
// ################################################################################

/// @brief Contains fundamental types definitions
/// @author Raffaele D. Facendola - April 2023

// ################################################################################

module;

#include <cstdint>
#include <cstddef>
#include <memory>

// ################################################################################

export module syntropy.types;

// ################################################################################

export namespace sy
{
    // ================================================================================
    // FUNDAMENTAL TYPES
    // ================================================================================

    // Boolean type.
    using Bool = bool;

    // Integral type.
    using Int = std::int64_t;

    // Real type.
    using Real = float;

    // ================================================================================
    // FIXED-SIZE TYPES
    // ================================================================================

    // 8-bit integer type.
    enum class Fix8 : std::int8_t {};

    // 16-bit integer type.
    enum class Fix16 : std::int16_t {};

    // 32-bit integer type.
    enum class Fix32 : std::int32_t {};

    // 65-bit integer type.
    enum class Fix64 : std::int64_t {};

    // ================================================================================
    // ENUMERATION TYPES
    // ================================================================================

    // 8-bit enumeration type.
    using Enum8 = std::int8_t;

    // 16-bit enumeration type.
    using Enum16 = std::int16_t;

    // 32-bit enumeration type.
    using Enum32 = std::int32_t;

    // 64-bit enumeration type.
    using Enum64 = std::int64_t;

    // ================================================================================
    // POINTER TYPES
    // ================================================================================

    // Type of the nullptr constant.
    using Null = std::nullptr_t;

    // Address to read-only object.
    template <typename Type>
    using Address = const Type*;

    // Address to a read-write object.
    template <typename Type>
    using RWAddress = Type*;

    // Get an address to a read-only object.
    template <typename Type>
    constexpr Address<Type> AddressOf(const Type& rhs);

    // Get an address to a read-write object.
    template <typename Type>
    constexpr RWAddress<Type> AddressOf(Type& rhs);

    // ================================================================================
    // ACCESS
    // ================================================================================

    // Convert a reference to constant.
    template <typename Type>
    constexpr const Type& ToConst(const Type& rhs);

    // Convert a reference to non-constant.
    // If the original value is constant, accessing the returned value results in undefined behaviour.
    template <typename Type>
    constexpr Type& FromConst(const Type& rhs);

    // Convert an address to an object to an address to an immutable object.
    template <typename Type>
    constexpr Address<Type> ToReadOnly(Address<Type> rhs);

    // Convert an address to an object to an address to a mutable object.
    // If the original value is not read-writable, accessing the returned value results in undefined behaviour.
    template <typename Type>
    constexpr RWAddress<Type> ToReadWrite(Address<Type> rhs);

    // ================================================================================
    // CONVERSIONS
    // ================================================================================

    // Convert a Fix8 to an Int.
    constexpr Int ToInt(Fix8 rhs);

    // Convert a Fix16 to an Int.
    constexpr Int ToInt(Fix16 rhs);

    // Convert a Fix32 to an Int.
    constexpr Int ToInt(Fix32 rhs);

    // Convert a Fix64 to an Int.
    constexpr Int ToInt(Fix64 rhs);

    // Convert an address to its numerical representation.
    template <typename Type>
    Int ToInt(Address<Type> rhs);

}

// ################################################################################

// ================================================================================
// IMPLEMENTATION
// ================================================================================

template <typename Type>
constexpr sy::Address<Type> sy::AddressOf(const Type& rhs)
{
    return std::addressof(rhs);
}

template <typename Type>
constexpr sy::RWAddress<Type> sy::AddressOf(Type& rhs)
{
    return std::addressof(rhs);
}

template <typename Type>
constexpr const Type& sy::ToConst(const Type& rhs)
{
    return rhs;
}

template <typename Type>
constexpr Type& sy::FromConst(const Type& rhs)
{
    return const_cast<Type&>(rhs);
}

template <typename Type>
constexpr sy::Address<Type> sy::ToReadOnly(Address<Type> rhs)
{
    return rhs;
}

template <typename Type>
constexpr sy::RWAddress<Type> sy::ToReadWrite(Address<Type> rhs)
{
    return const_cast<RWAddress<Type>>(rhs);
}

template <typename Type>
sy::Int sy::ToInt(Address<Type> rhs)
{
    return reinterpret_cast<Int>(rhs);
}

// ################################################################################
