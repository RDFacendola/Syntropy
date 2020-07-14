#include "syntropy/memory/memory.h"

#include "syntropy/diagnostics/assert.h"
#include "syntropy/math/math.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    Bytes Memory::Copy(const MemorySpan& destination, const ReadOnlyMemorySpan& source)
    {
        auto source_size = source.GetSize();
        auto destination_size = destination.GetSize();

        auto copy_size = Math::Min(source_size, destination_size);

        if (copy_size > Bytes{ 0 })
        {
            auto destination_span = ReadOnlyMemorySpan(destination.GetData(), copy_size);
            auto source_span = ReadOnlyMemorySpan(source.GetData(), copy_size);

            if (!Overlaps(destination_span, source_span))
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

    Bytes Memory::Gather(const MemorySpan& destination, InitializerList<ReadOnlyMemorySpan> sources)
    {
        auto gather = destination;

        for (auto&& source : sources)
        {
            auto count = Copy(gather, source);

            PopFront(gather, count);
        }

        return MemorySpan{ destination.GetData(), gather.GetData() }.GetSize();
    }

    Bytes Memory::Scatter(InitializerList<MemorySpan> destinations, const ReadOnlyMemorySpan& source)
    {
        auto scatter = source;

        for (auto&& destination : destinations)
        {
            auto count = Copy(destination, scatter);

            PopFront(scatter, count);
        }

        return ReadOnlyMemorySpan{ source.GetData(), scatter.GetData() }.GetSize();
    }

}