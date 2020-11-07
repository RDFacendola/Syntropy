
/// \file bytes_details.h
/// \brief This header is part of Syntropy memory module. It contains implementation details of bytes-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/templates/traits.h"

namespace Syntropy::Memory
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename TRatio>
    class BytesT;
}

namespace Syntropy::Memory::Templates::Details
{
    /************************************************************************/
    /* IS BYTES                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TBytes is a BytesT<T> type, equal to false otherwise. Defaults to false.
    template <typename TBytes>
    constexpr bool IsBytes = false;

    /// \brief Specialization for TBytes<TUnit>. Defaults to true.
    template <typename TRatio>
    constexpr bool IsBytes<BytesT<TRatio>> = true;

    /************************************************************************/
    /* BYTE RATIO                                                           */
    /************************************************************************/

    /// \brief If TBytes is a BytesT<TRatio> type, exposes a type alias TType equal to TRatio, otherwise there's no such type.
    template <typename TBytes>
    struct ByteRatio
    {
        static_assert(AlwaysFalse<TBytes>, "TBytes is not a BytesT<> type.");
    };

    /// \brief Partial template specialization for BytesT<TRatio>.
    template <typename TRatio>
    struct ByteRatio<BytesT<TRatio>>
    {
        using Type = TRatio;
    };
}