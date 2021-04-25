
/// \file byte.inl
///
/// \author Raffaele D. Facendola - August 2020.

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Conversion.
    // ===========

    template <typename TNumber>
    [[nodiscard]] constexpr Byte
    ToByte(TNumber rhs) noexcept
    {
        return static_cast<Byte>(rhs);
    }

    template <typename TType>
    [[nodiscard]] inline BytePtr
    ToBytePtr(Ptr<TType> rhs) noexcept
    {
        return reinterpret_cast<BytePtr>(rhs);
    }

    template <typename TType>
    [[nodiscard]] inline RWBytePtr
    ToBytePtr(RWPtr<TType> rhs) noexcept
    {
        return const_cast<RWBytePtr>(reinterpret_cast<BytePtr>(rhs));
    }

    template <typename TType>
    [[nodiscard]] inline Ptr<TType>
    FromBytePtr(BytePtr rhs) noexcept
    {
        return reinterpret_cast<Ptr<TType>>(rhs);
    }

    template <typename TType>
    [[nodiscard]] inline RWPtr<TType>
    FromBytePtr(RWBytePtr rhs) noexcept
    {
        return const_cast<RWPtr<TType>>(reinterpret_cast<Ptr<TType>>(rhs));
    }
}

// ===========================================================================
