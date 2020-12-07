
/// \file bytes_details.h
/// \brief This header is part of Syntropy memory module. It contains implementation details of bytes-related type traits.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

#include "syntropy/language/templates/type_traits.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* FORWARD DECLARATIONS                                                 */
    /************************************************************************/

    template <typename TRatio>
    class DataSize;
}

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* IS BYTES                                                             */
    /************************************************************************/

    /// \brief Constant equal to true if TBytes is a DataSize<T> type, equal to false otherwise. Defaults to false.
    template <typename TBytes>
    constexpr bool IsBytes = false;

    /// \brief Specialization for TBytes<TUnit>. Defaults to true.
    template <typename TRatio>
    constexpr bool IsBytes<DataSize<TRatio>> = true;

    /************************************************************************/
    /* BYTE RATIO                                                           */
    /************************************************************************/

    /// \brief If TBytes is a DataSize<TRatio> type, exposes a type alias TType equal to TRatio, otherwise there's no such type.
    template <typename TBytes>
    struct ByteRatioHelper
    {
        static_assert(AlwaysFalse<TBytes>, "TBytes is not a DataSize<> type.");
    };

    /// \brief Partial template specialization for DataSize<TRatio>.
    template <typename TRatio>
    struct ByteRatioHelper<DataSize<TRatio>>
    {
        using Type = TRatio;
    };

    /// \brief If TBytes is DataSize<TRatio> this type alias is equal to TRatio, otherwise the program is ill-formed.
    template <typename TBytes>
    using ByteRatio = typename ByteRatioHelper<TBytes>::Type;
}

// ===========================================================================
