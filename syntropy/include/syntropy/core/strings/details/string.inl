
/// \file string.inl
///
/// \author Raffaele D. Facendola - February 2021.

// ===========================================================================

#include "syntropy/language/foundation/foundation.h"

#include "syntropy/core/ranges/sized_range.h"
#include "syntropy/core/comparisons/compare.h"
#include "syntropy/core/toolset/swap.h"

#include "syntropy/memory/allocators/allocator.h"
#include "syntropy/memory/foundation/buffer.h"
#include "syntropy/memory/foundation/memory.h"
#include "syntropy/memory/foundation/byte_span.h"

namespace Syntropy::StringsADL
{
    /************************************************************************/
    /* STRING EXTENSIONS                                                    */
    /************************************************************************/

    /// \brief Type alias for the ToString extension functor.
    template <typename TType>
    using ToStringExtension
        = Syntropy::Strings::Extensions::ToString<TType>;

    /// \brief Invoke the ToString function via extension functor.
    template <typename TType>
    [[nodiscard]] auto
    InvokeToStringExtension(Immutable<TType> rhs)
        noexcept -> decltype(ToStringExtension<TType>{}(rhs))
    {
        return ToStringExtension<TType>{}(rhs);
    }

    /************************************************************************/
    /* STRINGS                                                              */
    /************************************************************************/

    /// \brief Convert a string literal to a string.
    template <Int TSize>
    inline String
    ToString(StringLiteral<TSize> rhs) noexcept
    {
        return { rhs };
    }

    /// \brief Convert a boolean to a string.
    template <Templates::IsBoolean TBoolean>
    inline Immutable<String>
    ToString(TBoolean rhs) noexcept
    {
        static const auto kTrueString = String{u8"true"};
        static const auto kFalseString = String{u8"false"};

        return rhs ? kTrueString : kFalseString;
    }

    /// \brief Convert an integral to a string.
    template <Templates::IsIntegral TIntegral>
    inline String
    ToString(TIntegral rhs) noexcept
    {
        return u8"[integral]";  // #TODO @rfacendola Implement me!
    }

    /// \brief Convert a boolean to a string.
    template <Templates::IsReal TReal>
    inline String
    ToString(TReal rhs) noexcept
    {
        return u8"[real]";      // #TODO @rfacendola Implement me!
    }

    /// \brief Invoke the ToString function, trying different implementations.
    template <typename TType>
    constexpr decltype(auto)
    InvokeToString(Immutable<TType> rhs) noexcept
    {
        auto extension = [](auto&& rhs)
            -> decltype(InvokeToStringExtension(rhs))
        {
            return InvokeToStringExtension(rhs);
        };

        auto member_function = [](auto&& rhs)
            -> decltype(rhs.ToString())
        {
            return rhs.ToString();
        };

        auto non_member_function = [](auto&& rhs)
            -> decltype(ToString(rhs))
        {
            return ToString(rhs);
        };

        return Templates::InvokeAny(extension,
                                    member_function,
                                    non_member_function)(rhs);
    }
}

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* STRING                                                               */
    /************************************************************************/

    inline String
    ::String (Null) noexcept
        : String()
    {

    }

    template <Int TSize>
    String
    ::String(StringLiteral<TSize> characters) noexcept
        : code_units_(Memory::ToBytes(TSize))
    {
        Memory::Copy(code_units_,
                     Memory::MakeByteSpan(characters));
    }

    inline
    String
    ::operator StringView() const noexcept
    {
        return StringView{ code_units_ };
    }

    [[nodiscard]] inline Memory::ByteSpan
    String
    ::GetCodeUnits() const noexcept
    {
        return ViewOf(code_units_);
    }

    [[nodiscard]] inline Mutable<Memory::BaseAllocator>
    String
    ::GetAllocator() const noexcept
    {
        return code_units_.GetAllocator();
    }

    inline void
    String
    ::Swap(Movable<String> rhs) noexcept
    {
        Toolset::Swap(code_units_, rhs.code_units_);
    }

    /************************************************************************/
    /* STRINGS                                                              */
    /************************************************************************/

    /// \brief Convert rhs to a string.
    template <typename TType>
    [[nodiscard]] constexpr decltype(auto)
    Strings
    ::ToString(Immutable<TType> rhs) noexcept
    {
        return StringsADL::InvokeToString(rhs);
    }

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    inline Bool
    operator==(Immutable<String> lhs, Immutable<String> rhs) noexcept
    {
        return ViewOf(lhs) == ViewOf(rhs);
    }

    template <Int TSize>
    inline Bool
    operator==(Immutable<String> lhs, StringLiteral<TSize> rhs) noexcept
    {
        return ViewOf(lhs) == rhs;
    }

    template <Int TSize>
    inline Bool
    operator==(StringLiteral<TSize> lhs, Immutable<String> rhs) noexcept
    {
        return lhs == ViewOf(rhs);
    }

    // Ranges.
    // =======

    [[nodiscard]] inline StringView
    ViewOf(Immutable<String> string) noexcept
    {
        return string;
    }

}

// ===========================================================================
