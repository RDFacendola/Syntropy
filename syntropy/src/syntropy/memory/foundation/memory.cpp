
/// \file memory.cpp
///
/// \author Raffaele D. Facendola - February 2021

#include "syntropy/memory/foundation/memory.h"

#include "syntropy/core/ranges/contiguous_range.h"

#include "syntropy/math/math.h"

// ===========================================================================

namespace Syntropy::Memory
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    Bytes
    Copy(Immutable<RWByteSpan> destination, Immutable<ByteSpan> source)
    {
        auto size = Math::Min(source.GetCount(), destination.GetCount());

        if (size > Bytes{ 0 })
        {
            // auto destination_span = Ranges::Front(destination, size);

            // auto source_span = Ranges::Front(source, size);

            // if (!Ranges::Intersect(destination_span, source_span))
            {
                // Faster copy for non-overlapping ranges.

                std::memcpy(destination.GetData(),
                            source.GetData(),
                            ToInt(size));
            }
            //else
            {
                // Slower copy for overlapping ranges.

                std::memmove(destination.GetData(),
                             source.GetData(),
                             ToInt(size));
            }
        }

        return size;
    }
}

// ===========================================================================
