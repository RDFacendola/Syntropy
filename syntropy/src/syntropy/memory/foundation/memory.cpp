
/// \file memory.cpp
///
/// \author Raffaele D. Facendola - February 2021

#include "syntropy/memory/foundation/memory.h"

#include "syntropy/core/ranges/random_access_range.h"

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
        // auto copy_size = Math::Min(source.GetCount(), destination.GetCount());
        //
        // if (copy_size > Bytes{ 0 })
        // {
        //     auto destination_span = Ranges::Front(destination,
        //                                           ToInt(copy_size));
        //
        //     auto source_span = Ranges::Front(source,
        //                                      ToInt(copy_size));
        //
        //     if (!Ranges::Intersection(destination_span, source_span))
        //     {
        //         // Faster copy for non-overlapping ranges.
        //
        //         std::memcpy(destination.GetData(),
        //                     source.GetData(),
        //                     ToInt(copy_size));
        //     }
        //     else
        //     {
        //         // Slower copy for overlapping ranges.
        //
        //         std::memmove(destination.GetData(),
        //                      source.GetData(),
        //                      ToInt(copy_size));
        //     }
        // }
        //
        // return copy_size;

        return {};
    }
}

// ===========================================================================
