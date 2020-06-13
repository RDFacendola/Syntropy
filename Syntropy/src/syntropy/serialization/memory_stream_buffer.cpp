#include "syntropy/serialization/memory_stream_buffer.h"

namespace syntropy
{
    /************************************************************************/
    /* MEMORY STREAM BUFFER                                                 */
    /************************************************************************/

    ConstMemoryRange MemoryStreamBuffer::Append(const ConstMemoryRange& data)
    {
        Grow(data.GetSize());

        auto write_position = size_;

        size_ += data.GetSize();

        return Write(write_position, data);                                                                                                     // Returned range is always empty.
    }

    MemoryRange MemoryStreamBuffer::Consume(const MemoryRange& data)
    {
        auto read_range = Read(Bytes{ 0 }, data);

        base_pointer_ = GetAddress(read_range.GetSize());

        size_ -= read_range.GetSize();

        return read_range;
    }

    ConstMemoryRange MemoryStreamBuffer::Write(Bytes position, const ConstMemoryRange& data)
    {
        auto written_data = [this, position, &data]()
        {
            auto source = UpperBound(data, size_ - position);                                                                                   // Limit writable data to current buffer size.

            auto destination_begin = GetAddress(position);
            auto destination_end = GetAddress(position + source.GetSize());

            if (destination_begin < destination_end)
            {
                return Memory::Copy({ destination_begin, destination_end }, source);                                                            // Contiguous range.
            }
            else
            {
                return Memory::Scatter({ { destination_begin, buffer_.End() }, { buffer_.Begin(), destination_end } }, source);                 // Wrap-around range.
            }
        }();

        return { data.Begin() + written_data, data.End() };
    }

    MemoryRange MemoryStreamBuffer::Read(Bytes position, const MemoryRange& data)
    {
        auto read_data = [this, position, &data]()
        {
            auto destination = UpperBound(data, size_ - position);                                                                              // Limit readable data to current buffer size.

            auto source_begin = GetAddress(position);
            auto source_end = GetAddress(position + destination.GetSize());

            if (source_begin < source_end)
            {
                return Memory::Copy(destination, { source_begin, source_end });                                                                 // Contiguous range.
            }
            else
            {
                return Memory::Gather(destination, { { source_begin, buffer_.End() }, { buffer_.Begin(), source_end } });                       // Wrap-around range.
            }
        }();

        return { data.Begin(), read_data };
    }

    void MemoryStreamBuffer::Grow(Bytes size)
    {
        if (auto capacity = GetSize() + size; capacity > GetCapacity())
        {
            capacity = ToBytes(Math::CeilTo<Int>((*capacity) * kGrowthFactor + kGrowthBias));                                                   // Exponential growth to avoid continuous reallocations.

            Realloc(capacity);
        }
    }

    void MemoryStreamBuffer::Realloc(Bytes capacity)
    {
        auto buffer = MemoryBuffer{ capacity, buffer_.GetMemoryResource() };

        if ((buffer_.GetSize() > Bytes{ 0 }) && (capacity > Bytes{ 0 }))
        {
            auto head_pointer = GetAddress(size_);

            if (base_pointer_ < head_pointer)
            {
                Memory::Copy(buffer.GetData(), { base_pointer_, head_pointer });                                                                // Contiguous range.
            }
            else
            {
                Memory::Gather(buffer.GetData(), { { base_pointer_, buffer_.End() }, { buffer_.Begin(), head_pointer } });                      // Wrap-around range.
            }

        }

        buffer_.Swap(buffer);

        base_pointer_ = buffer_.Begin();
    }

    MemoryAddress MemoryStreamBuffer::GetAddress(Bytes offset)
    {
        offset = (base_pointer_ + offset - buffer_.Begin());        // Advance.

        offset = ToBytes(offset % buffer_.GetSize());               // Wrap-around.

        return buffer_.Begin() + offset;                            // Offset in buffer-space.
    }

}