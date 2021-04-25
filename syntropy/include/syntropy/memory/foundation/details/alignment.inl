
/// \file alignment.inl
///
/// \author Raffaele D. Facendola - August 2018.

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    template <typename TType>
    [[nodiscard]] constexpr Alignment
    Memory
    ::AlignmentOf(Immutable<TType> rhs) noexcept
    {
        return Alignment(std::align_val_t{ alignof(TType) });
    }

    template <typename TType>
    [[nodiscard]] constexpr Alignment
    Memory
    ::AlignmentOf() noexcept
    {
        return Alignment(std::align_val_t{ alignof(TType) });
    }

    [[nodiscard]] constexpr Alignment
    Memory
    ::MaxAlignment() noexcept
    {
        return Alignment(std::align_val_t{ alignof(std::max_align_t) });
    }

    [[nodiscard]] inline Bool
    Memory
    ::IsAlignedTo(BytePtr pointer, Alignment alignment) noexcept
    {
        auto mask = ToInt(alignment) - 1;

        return !(ToAddress(pointer) & mask);
    }

    [[nodiscard]] inline BytePtr
    Memory
    ::Align(BytePtr pointer, Alignment alignment) noexcept
    {
        auto mask = ToInt(alignment) - 1;

        auto aligned = (ToAddress(pointer) + mask) & ~mask;

        return FromAddress<Byte>(aligned);
    }

    [[nodiscard]] inline RWBytePtr
    Memory
    ::Align(RWBytePtr pointer, Alignment alignment) noexcept
    {
        using Syntropy::ToReadOnly;
        using Syntropy::ToReadWrite;

        return ToReadWrite(Align(ToReadOnly(pointer), alignment));
    }

    [[nodiscard]] inline BytePtr
    Memory
    ::AlignDown(BytePtr pointer, Alignment alignment) noexcept
    {
        auto mask = ToInt(alignment) - 1;

        auto aligned_pointer = ToAddress(pointer) & ~mask;

        return FromAddress<Byte>(aligned_pointer);
    }

    [[nodiscard]] inline RWBytePtr
    Memory
    ::AlignDown(RWBytePtr pointer, Alignment alignment) noexcept
    {
        using Syntropy::ToReadOnly;
        using Syntropy::ToReadWrite;

        return ToReadWrite(AlignDown(ToReadOnly(pointer), alignment));
    }

    /************************************************************************/
    /* MEMORY LITERALS                                                      */
    /************************************************************************/

    [[nodiscard]] constexpr Alignment
    MemoryLiterals
    ::operator "" _Alignment(IntLiteral lhs) noexcept
    {
        return Alignment(lhs);
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    [[nodiscard]] constexpr Mutable<Alignment>
    operator>>=(Mutable<Alignment> lhs, Int rhs) noexcept
    {
        lhs = lhs >> rhs;

        return lhs;
    }

    [[nodiscard]] constexpr Mutable<Alignment>
    operator<<=(Mutable<Alignment> lhs, Int rhs) noexcept
    {
        lhs = lhs << rhs;

        return lhs;
    }

    [[nodiscard]] constexpr Alignment
    operator>>(Immutable<Alignment> lhs, Int rhs) noexcept
    {
        return (rhs >= 0) ? ToAlignment(ToInt(lhs) >> rhs) : (lhs << -rhs);
    }

    [[nodiscard]] constexpr Alignment
    operator<<(Immutable<Alignment> lhs, Int rhs) noexcept
    {
        return (rhs >= 0) ? ToAlignment(ToInt(lhs) << rhs) : (lhs >> -rhs);
    }

    [[nodiscard]] constexpr Int
    ToInt(Alignment lhs) noexcept
    {
        return static_cast<Int>(lhs);
    }

    [[nodiscard]] constexpr Bytes
    ToBytes(Alignment lhs) noexcept
    {
        return ToBytes(ToInt(lhs));
    }

    [[nodiscard]] constexpr Alignment
    ToAlignment(Int lhs) noexcept
    {
        return Alignment(lhs);
    }

    [[nodiscard]] constexpr Alignment
    ToAlignment(Bytes lhs) noexcept
    {
        return ToAlignment(ToInt(lhs));
    }

}

// ===========================================================================
