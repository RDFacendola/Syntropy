
/// \file memory.h
/// \brief This header is part of the Syntropy memory module. It contains low-level memory functionalities.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <cstring>
#include <algorithm>

#include "syntropy/language/type_traits.h"
#include "syntropy/language/initializer_list.h"

#include "syntropy/core/types.h"
#include "syntropy/core/span.h"

#include "syntropy/memory/byte_span.h"
#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"

#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    /// \brief Exposes methods used to manipulate memory.
    namespace Memory
    {

        /// \brief Reinterpret an object representation from a type to another type.
        template <typename TTo, typename TFrom>
        TTo BitCast(const TFrom& rhs);

        /// \brief Convert a pointer to its numeric address value.
        template <typename TType>
        Int NumericAddress(Pointer<TType> pointer) noexcept;

        /// \brief Copy a source memory region to a destination memory region. Neither span is exceed during the process.
        /// \return Returns the bytes copied as a result of this call.
        Bytes Copy(const RWByteSpan& destination, const ByteSpan& source);

        /// \brief Copy a source memory region to a destination memory region repeating the source until destination span is exhausted. Neither span is exceed during the process.
        void Repeat(const RWByteSpan& destination, const ByteSpan& source);

        /// \brief Set a value to each byte in a destination span.
        void Set(const RWByteSpan& destination, Byte value);

        /// \brief Zero-out a memory region.
        void Zero(const RWByteSpan& destination);

        /// \brief Gather data from one or more memory regions to fill a contiguous memory region sequentially. Neither span is exceeded during the process.
        /// \return Returns the bytes copied as a result of this call.
        Bytes Gather(const RWByteSpan& destination, InitializerList<ByteSpan> sources);

        /// \brief Scatter a contiguous memory region to one or more memory regions sequentially. Neither span is exceeded during the process.
        /// \return Returns the bytes copied as a result of this call.
        Bytes Scatter(InitializerList<RWByteSpan> destinations, const ByteSpan& source);

    };

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // Memory.
    // =======

    template <typename TTo, typename TFrom>
    inline TTo Memory::BitCast(const TFrom& rhs)
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
    inline Int Memory::NumericAddress(Pointer<TType> pointer) noexcept
    {
        return reinterpret_cast<Int>(pointer);
    }

    inline void Memory::Repeat(const RWByteSpan& destination, const ByteSpan& source)
    {
        for (auto span = destination; !IsEmpty(span);)
        {
            auto count = Copy(span, source);

            span = PopFront(span, ToInt(count));
        }
    }

    inline void Memory::Set(const RWByteSpan& destination, Byte value)
    {
        std::memset(destination.GetData(), static_cast<int>(value), ToInt(Size(destination)));
    }

    inline void Memory::Zero(const RWByteSpan& destination)
    {
        Set(destination, Byte{ 0 });
    }

}
