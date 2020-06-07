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

    Bytes Memory::CopyUnfold(const MemoryRange& destination, const ConstMemoryRange& source, Bytes source_offset)
    {
        auto source_size = source.GetSize();
        auto destination_size = destination.GetSize();

        auto copy_size = Math::Min(source_size, destination_size);

        if (copy_size > Bytes{ 0 })
        {
            source_offset %= (*source_size);

            auto [source_tail, source_head] = Split(source, source_offset);

            auto source_head_size = Math::Min(source_head.GetSize(), destination.GetSize());        // Prevent buffer overruns if the source is larger than the destination.

            auto [destination_head, destination_tail] = Split(destination, source_head_size);

            Memory::Copy(destination_head, source_head);
            Memory::Copy(destination_tail, source_tail);
        }

        return copy_size;
    }

    Bytes Memory::CopyFold(const MemoryRange& destination, const ConstMemoryRange& source, Bytes destination_offset)
    {
        auto source_size = source.GetSize();
        auto destination_size = destination.GetSize();

        auto copy_size = Math::Min(source_size, destination_size);

        if (copy_size > Bytes{ 0 })
        {
            destination_offset %= (*destination_size);

            auto [destination_tail, destination_head] = Split(destination, destination_offset);

            auto destination_head_size = Math::Min(destination_head.GetSize(), source.GetSize());   // Prevent buffer overruns if the source is larger than the destination.

            auto [source_head, source_tail] = Split(source, destination_head_size);

            Memory::Copy(destination_head, source_head);
            Memory::Copy(destination_tail, source_tail);
        }

        return copy_size;
    }
}