
/// \file bytes_details.h
/// \brief This header is part of Syntropy memory module. It contains implementation details of bytes-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/templates/constants.h"

namespace Syntropy
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename TUnit>
    class BytesT;
}

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* IS BYTES                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TBytes is a BytesT<T> type, equal to false otherwise. Defaults to false.
    template <typename TRational>
    constexpr bool IsBytes = false;

    /// \brief Specialization for TBytes<TUnit>. Defaults to true.
    template <typename TUnit>
    constexpr bool IsBytes<BytesT<TUnit>> = true;

    /************************************************************************/
    /* BYTE UNITS                                                           */
    /************************************************************************/

    /// \brief If TBytes is a BytesT<TUnit> type, exposes a type alias TType equal to TUnit, otherwise there's no such alias.
    template <typename TBytes>
    struct ByteUnits
    {
        static_assert(AlwaysFalse<TBytes>, "TBytes is not a BytesT<> type.");
    };

    /// \brief Partial template specialization for BytesT<TUnit>.
    template <typename TUnit>
    struct ByteUnits<BytesT<TUnit>>
    {
        using TType = TUnit;
    };
}