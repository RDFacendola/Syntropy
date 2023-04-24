
// ################################################################################

// @author Raffaele D. Facendola - April 2023

// ################################################################################

module;

#include <cstddef>

// ################################################################################

module syntropy.memory;

import syntropy.types;

// ################################################################################

// ================================================================================
// ARITHMETIC
// ================================================================================

sy::Bytes sy::operator+(Bytes lhs, Bytes rhs)
{
    return sy::Bytes{ ToInt(lhs) + ToInt(rhs) };
}

sy::Bytes sy::operator-(Bytes lhs, Bytes rhs)
{
    return sy::Bytes{ ToInt(lhs) - ToInt(rhs) };
}

sy::Address<sy::Byte> sy::operator+(Address<Byte> lhs, Bytes rhs)
{
    return lhs + ToInt(rhs);
}

sy::Address<sy::Byte> sy::operator-(Address<Byte> lhs, Bytes rhs)
{
    return lhs - ToInt(rhs);
}

sy::RWAddress<sy::Byte> sy::operator+(RWAddress<Byte> lhs, Bytes rhs)
{
    return lhs + ToInt(rhs);
}

sy::RWAddress<sy::Byte> sy::operator-(RWAddress<Byte> lhs, Bytes rhs)
{
    return lhs - ToInt(rhs);
}

// ================================================================================
// ALIGNMENT
// ================================================================================

constexpr sy::Alignment sy::GetMaxAlignment()
{
    return Alignment(std::align_val_t{ alignof(std::max_align_t) });
}

sy::Address<sy::Byte> sy::Align(Address<Byte> lhs, Alignment rhs)
{
    auto address = ToInt(lhs);
    auto mask = ToInt(rhs) - 1;

    auto aligned = (address + mask) & (~mask);

    return reinterpret_cast<Address<Byte>>(aligned);
}

sy::RWAddress<sy::Byte> sy::Align(RWAddress<Byte> lhs, Alignment rhs)
{
    return ToReadWrite(Align(ToReadOnly(lhs), rhs));
}

// ================================================================================
// CONVERSIONS
// ================================================================================

constexpr sy::Int sy::ToInt(Byte rhs)
{
    return static_cast<sy::Int>(rhs);
}

constexpr sy::Int sy::ToInt(Bytes rhs)
{
    return static_cast<sy::Int>(rhs);
}

constexpr sy::Int sy::ToInt(KiBytes rhs)
{
    return static_cast<sy::Int>(rhs);
}

constexpr sy::Int sy::ToInt(MiBytes rhs)
{
    return static_cast<sy::Int>(rhs);
}

constexpr sy::Int sy::ToInt(GiBytes rhs)
{
    return static_cast<sy::Int>(rhs);
}

constexpr sy::Bytes sy::ToBytes(KiBytes rhs)
{
    return static_cast<Bytes>(ToInt(rhs) * 1024);
}

constexpr sy::Bytes sy::ToBytes(MiBytes rhs)
{
    return static_cast<Bytes>(ToInt(rhs) * 1024 * 1024);
}

constexpr sy::Bytes sy::ToBytes(GiBytes rhs)
{
    return static_cast<Bytes>(ToInt(rhs) * 1024 * 1024 * 1024);
}

constexpr sy::KiBytes sy::ToKiBytes(MiBytes rhs)
{
    return static_cast<KiBytes>(ToInt(rhs) * 1024);
}

constexpr sy::KiBytes sy::ToKiBytes(GiBytes rhs)
{
    return static_cast<KiBytes>(ToInt(rhs) * 1024 * 1024);
}

constexpr sy::MiBytes sy::ToMiBytes(GiBytes rhs)
{
    return static_cast<MiBytes>(ToInt(rhs) * 1024);
}

constexpr sy::Int sy::ToInt(Alignment rhs)
{
    return static_cast<sy::Int>(rhs);
}

// ################################################################################
