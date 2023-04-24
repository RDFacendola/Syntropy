
// ################################################################################

// @brief Contains basic memory definitions.
// @author Raffaele D. Facendola - April 2023

// ################################################################################

module;

#include <memory>

// ################################################################################

export module syntropy.memory;

import syntropy.types;
import syntropy.span;

// ################################################################################

export namespace sy
{
    // ================================================================================
    // MEMORY TYPES
    // ================================================================================

    // Represents a Byte. This type is neither a character type, nor an arithmetic one.
    enum class Byte : Enum8 {};

    // Memory size, expressed as Bytes.
    enum class Bytes : Enum64 {};

    // Memory size, expressed as KiBytes.
    enum class KiBytes : Enum64 {};

    // Memory size, expressed as MiBytes.
    enum class MiBytes : Enum64 {};

    // Memory size, expressed as GiBytes.
    enum class GiBytes : Enum64 {};

    // Represents a memory alignment.
    enum class Alignment : Enum64 {};

    // A read-only span of bytes.
    using ByteSpan = Span<Byte>;

    // A read-write span of bytes.
    using RWByteSpan = RWSpan<Byte>;

    // ================================================================================
    // ARITHMETIC
    // ================================================================================

    // Sum of two Bytes amount.
    Bytes operator+(Bytes lhs, Bytes rhs);

    // Difference of two Bytes amount.
    Bytes operator-(Bytes lhs, Bytes rhs);

    // Advance a byte address forward.
    Address<Byte> operator+(Address<Byte> lhs, Bytes rhs);

    // Advance a byte address backward.
    Address<Byte> operator-(Address<Byte> lhs, Bytes rhs);

    // Advance a byte address forward.
    RWAddress<Byte> operator+(RWAddress<Byte> lhs, Bytes rhs);

    // Advance a byte address backward.
    RWAddress<Byte> operator-(RWAddress<Byte> lhs, Bytes rhs);

    // ================================================================================
    // QUERY
    // ================================================================================

    // Get the size of rhs, in Bytes.
    template <typename Type>
    constexpr Bytes SizeOf(const Type& rhs);

    // Get the size of Type, in Bytes.
    template <typename Type>
    constexpr Bytes SizeOf();

    // Get the alignment of rhs.
    template <typename Type>
    constexpr Alignment AlignmentOf(const Type& rhs);

    // Get the alignment of Type.
    template <typename Type>
    constexpr Alignment AlignmentOf();

    // Get the maximum alignment any scalar type can be aligned to.
    constexpr Alignment GetMaxAlignment();

    // ================================================================================
    // ALIGNMENT
    // ================================================================================

    // Check whether an address is aligned to a given value.
    template <typename Type>
    Bool IsAligned(Address<Type> lhs, Alignment rhs);

    // Check whether an byte address is aligned to a given value.
    template <typename Type>
    Bool IsAligned(RWAddress<Type> lhs, Alignment rhs);

    // Align a byte address to a given value.
    Address<Byte> Align(Address<Byte> lhs, Alignment rhs);

    // Align a byte address to a given value.
    RWAddress<Byte> Align(RWAddress<Byte> lhs, Alignment rhs);

    // ================================================================================
    // MARSHALLING
    // ================================================================================

    // Get the read-only representation of an object.
    template <typename Type>
    constexpr ByteSpan Marshal(const Type& rhs);

    // Get the read-write representation of an object.
    template <typename Type>
    constexpr RWByteSpan Marshal(Type& rhs);

    // ================================================================================
    // CONVERSIONS
    // ================================================================================

    // Convert a Byte to its integral representation.
    constexpr Int ToInt(Byte rhs);

    // Convert a KiBytes to Int.
    constexpr Int ToInt(Bytes rhs);

    // Convert a KiBytes to Int.
    constexpr Int ToInt(KiBytes rhs);

    // Convert a MiBytes to Int.
    constexpr Int ToInt(MiBytes rhs);

    // Convert a GiBytes to Int.
    constexpr Int ToInt(GiBytes rhs);

    // Convert a KiBytes to Bytes.
    constexpr Bytes ToBytes(KiBytes rhs);

    // Convert a MiBytes to Bytes.
    constexpr Bytes ToBytes(MiBytes rhs);

    // Convert a GiBytes to Bytes.
    constexpr Bytes ToBytes(GiBytes rhs);

    // Convert a MiBytes to KiBytes.
    constexpr KiBytes ToKiBytes(MiBytes rhs);

    // Convert a GiBytes to KiBytes.
    constexpr KiBytes ToKiBytes(GiBytes rhs);

    // Convert a GiBytes to MiBytes.
    constexpr MiBytes ToMiBytes(GiBytes rhs);

    // Convert an Alignment to Int.
    constexpr Int ToInt(Alignment rhs);

}

// ================================================================================
// IMPLEMENTATION
// ================================================================================

template <typename Type>
constexpr sy::Bytes sy::SizeOf(const Type& rhs)
{
    return Bytes{ sizeof(rhs) };
}

template <typename Type>
constexpr sy::Bytes sy::SizeOf()
{
    return Bytes{ sizeof(Type) };
}

template <typename Type>
constexpr sy::Alignment sy::AlignmentOf(const Type& rhs)
{
    return Alignment{ alignof(Type) };
}

template <typename Type>
constexpr sy::Alignment sy::AlignmentOf()
{
    return Alignment{ alignof(Type) };
}

template <typename Type>
sy::Bool sy::IsAligned(Address<Type> lhs, Alignment rhs)
{
    auto address = ToInt(lhs);
    auto mask = ToInt(rhs) - 1;

    return !(address & mask);
}

template <typename Type>
sy::Bool sy::IsAligned(RWAddress<Type> lhs, Alignment rhs)
{
    return IsAligned(ToReadOnly(lhs), rhs);
}

template <typename Type>
constexpr sy::ByteSpan sy::Marshal(const Type& rhs)
{
    auto data = reinterpret_cast<Address<Byte>>(AddressOf(rhs));
    auto count = ToInt(SizeOf(rhs));

    return { data, count };
}

template <typename Type>
constexpr sy::RWByteSpan sy::Marshal(Type& rhs)
{
    auto data = reinterpret_cast<RWAddress<Byte>>(AddressOf(rhs));
    auto count = ToInt(SizeOf(rhs));

    return { data, count };
}

// ################################################################################
