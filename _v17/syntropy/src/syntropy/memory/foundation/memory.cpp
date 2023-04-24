
/// \file memory.cpp
///
/// \author Raffaele D. Facendola - February 2021

// ===========================================================================

#include "syntropy/language/foundation/foundation.h"
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

        if (size > Syntropy::ToInt(0))
        {
            auto destination_span = Ranges::Front(destination, size);

            auto source_span = Ranges::Front(source, size);

            if (!Ranges::Intersect(destination_span, source_span))
            {
                // Faster copy for non-overlapping ranges.

                std::memcpy(Ranges::Data(destination_span),
                            Ranges::Data(source_span),
                            Syntropy::ToInt(size));
            }
            else
            {
                // Slower copy for overlapping ranges.

                std::memmove(Ranges::Data(destination_span),
                             Ranges::Data(source_span),
                             Syntropy::ToInt(size));
            }
        }

        return Bytes{ size };
    }
}

// ===========================================================================
