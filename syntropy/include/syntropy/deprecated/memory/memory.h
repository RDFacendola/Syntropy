
/// \file memory.h
/// \brief This header is part of the Syntropy memory module. It contains low-level memory functionalities.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstring>
#include <algorithm>

#include "syntropy/language/templates/traits.h"
#include "syntropy/language/support/initializer_list.h"

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/foundation/span.h"

#include "syntropy/memory/byte_span.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"

#include "syntropy/diagnostics/assert.h"

namespace Syntropy::Memory
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    /// \brief Reinterpret an object representation to another type.
    template <typename TTo, typename TFrom>
    TTo BitCast(const TFrom& rhs);

    /// \brief Convert a pointer to its numeric address value.
    template <typename TType>
    Int NumericAddressOf(Pointer<TType> pointer) noexcept;

    /// \brief Gather data from one or more memory regions to fill a contiguous memory region sequentially. Neither span is exceeded during the process.
    /// \return Returns the bytes copied as a result of this call.
    Bytes Gather(const RWByteSpan& destination, InitializerList<ByteSpan> sources);

    /// \brief Scatter a contiguous memory region to one or more memory regions sequentially. Neither span is exceeded during the process.
    /// \return Returns the bytes copied as a result of this call.
    Bytes Scatter(InitializerList<RWByteSpan> destinations, const ByteSpan& source);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Memory.
    // =======

    template <typename TTo, typename TFrom>
    inline TTo BitCast(const TFrom& rhs)
    {
        static_assert(sizeof(TTo) == sizeof(TFrom), "TTo and TFrom must have the same size.");
        static_assert(IsTriviallyCopyableV<TFrom>, "TFrom must be trivially copyable.");
        static_assert(IsTrivialV<TTo>, "TTo must be trivial.");
        static_assert(IsCopyConstructibleV<TTo> || IsMoveConstructibleV<TTo>, "TTo must either be copy constructible or move constructible.");
        static_assert(IsTriviallyDefaultConstructibleV<TTo>, "TTo must be trivially default constructible.");

        auto lhs = TTo{};

        std::memcpy(&lhs, &rhs, sizeof(TTo));

        return lhs;
    }

    template <typename TType>
    inline Int NumericAddressOf(Pointer<TType> pointer) noexcept
    {
        return reinterpret_cast<Int>(pointer);
    }

}
