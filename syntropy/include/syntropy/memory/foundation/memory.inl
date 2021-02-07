
/// \file memory.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include <cstring>

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    inline void
    Repeat(const RWByteSpan& destination, const ByteSpan& source)
    {
        for (auto span = destination; !IsEmpty(span);)
        {
            auto count = Copy(span, source);

            span = PopFront(span, ToInt(count));
        }
    }

    inline void
    Set(const RWByteSpan& destination, Byte value)
    {
        std::memset(destination.GetData(),
                    static_cast<int>(value),
                    ToInt(Size(destination)));
    }

    inline void
    Zero(const RWByteSpan& destination)
    {
        Set(destination, Byte{ 0 });
    }

}

// ===========================================================================
