
/// \file memory.inl
///
/// \author Raffaele D. Facendola - February 2021

#pragma once

#include <cstring>

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/ranges/forward_range.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    inline void
    Repeat(const RWByteSpan& destination, const ByteSpan& source)
    {
        // for (auto span = destination; !Ranges::IsEmpty(span);)
        {
            // auto count = Copy(span, source);

            // span = Ranges::PopFront(span, count);
        }
    }

    inline void
    Set(const RWByteSpan& destination, Byte value)
    {
        // std::memset(destination.GetData(),
        //            static_cast<int>(value),
        //            ToInt(Ranges::Count(destination)));
    }

    inline void
    Zero(const RWByteSpan& destination)
    {
        Set(destination, Byte{ 0 });
    }

}

// ===========================================================================
