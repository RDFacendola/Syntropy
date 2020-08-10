#include "syntropy/memory/memory.h"

#include "syntropy/diagnostics/assert.h"
#include "syntropy/math/math.h"

namespace Syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    Bytes Memory::Copy(const RWByteSpan& destination, const ByteSpan& source)
    {
        auto copy_size = Math::Min(Size(source), Size(destination));

        if (copy_size > Bytes{ 0 })
        {
            auto destination_span = Front(destination, ToInt(copy_size));
            auto source_span = Front(source, ToInt(copy_size));

            if (!Intersection(destination_span, source_span))
            {
                std::memcpy(destination.GetData(), source.GetData(), ToInt(copy_size));         // Faster copy for non-overlapping ranges.
            }
            else
            {
                std::memmove(destination.GetData(), source.GetData(), ToInt(copy_size));        // Slower copy for overlapping ranges.
            }
        }

        return copy_size;
    }

    Bytes Memory::Gather(const RWByteSpan& destination, InitializerList<ByteSpan> sources)
    {
        auto gather = destination;

        for (auto&& source : sources)
        {
            auto count = Copy(gather, source);

            PopFront(gather, ToInt(count));
        }

        return Size(ByteSpan{ destination.GetData(), gather.GetData() });
    }

    Bytes Memory::Scatter(InitializerList<RWByteSpan> destinations, const ByteSpan& source)
    {
        auto scatter = source;

        for (auto&& destination : destinations)
        {
            auto count = Copy(destination, scatter);

            PopFront(scatter, ToInt(count));
        }

        return Size(ByteSpan{ source.GetData(), scatter.GetData() });
    }

}