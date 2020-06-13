#include "syntropy/memory/memory.h"

#include "syntropy/diagnostics/assert.h"
#include "syntropy/math/math.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    Bytes Memory::Copy(const MemoryRange& destination, const ConstMemoryRange& source)
    {
        auto source_size = source.GetSize();
        auto destination_size = destination.GetSize();

        auto copy_size = Math::Min(source_size, destination_size);

        if (copy_size > Bytes{ 0 })
        {
            auto destination_range = ConstMemoryRange(destination.Begin(), copy_size);
            auto source_range = ConstMemoryRange(source.Begin(), copy_size);

            if (!Intersection(destination_range, source_range))
            {
                std::memcpy(*destination.Begin(), *source.Begin(), *copy_size);                     // Faster copy for non-overlapping ranges.
            }
            else
            {
                std::memmove(*destination.Begin(), *source.Begin(), *copy_size);                    // Slower copy for overlapping ranges.
            }
        }

        return copy_size;
    }

    Bytes Memory::Gather(const MemoryRange& destination, InitializerList<ConstMemoryRange> sources)
    {
        auto gather = destination;

        for (auto&& source : sources)
        {
            auto count = Copy(gather, source);

            gather.PopFront(count);
        }

        return MemoryRange{ destination.Begin(), gather.Begin() }.GetSize();
    }

    Bytes Memory::Scatter(InitializerList<MemoryRange> destinations, const ConstMemoryRange& source)
    {
        auto scatter = source;

        for (auto&& destination : destinations)
        {
            auto count = Copy(destination, scatter);

            scatter.PopFront(count);
        }

        return ConstMemoryRange{ source.Begin(), scatter.Begin() }.GetSize();
    }

}