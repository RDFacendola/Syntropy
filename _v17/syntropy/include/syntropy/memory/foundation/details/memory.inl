
/// \file memory.inl
///
/// \author Raffaele D. Facendola - February 2021.

// ===========================================================================

#include <cstring>

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/core/ranges/forward_range.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    inline void
    Memory
    ::Repeat(const RWByteSpan& destination, const ByteSpan& source)
    {
        // for (auto span = destination; !Ranges::IsEmpty(span);)
        {
            // auto count = Copy(span, source);

            // span = Ranges::PopFront(span, count);
        }
    }

    inline void
    Memory
    ::Set(const RWByteSpan& destination, Byte value)
    {
        // std::memset(destination.GetData(),
        //            static_cast<int>(value),
        //            ToInt(Ranges::Count(destination)));
    }

    inline void
    Memory
    ::Zero(const RWByteSpan& destination)
    {
        Set(destination, Byte{ 0 });
    }

}

// ===========================================================================
