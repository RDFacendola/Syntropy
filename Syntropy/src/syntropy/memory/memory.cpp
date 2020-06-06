#include "syntropy/memory/memory.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    Bytes Memory::Copy(const MemoryRange& destination, const ConstMemoryRange& source)
    {
        auto bytes = std::min(source.GetSize(), destination.GetSize());

        if (bytes > Bytes{ 0 })
        {
            auto destination_range = ConstMemoryRange(destination.Begin(), bytes);
            auto source_range = ConstMemoryRange(source.Begin(), bytes);

            if (!Intersection(destination_range, source_range))
            {
                std::memcpy(*destination.Begin(), *source.Begin(), bytes.GetCount());           // Faster copy for non-overlapping ranges.
            }
            else
            {
                std::memmove(*destination.Begin(), *source.Begin(), bytes.GetCount());          // Slower copy for overlapping ranges.
            }
        }

        return bytes;
    }
}