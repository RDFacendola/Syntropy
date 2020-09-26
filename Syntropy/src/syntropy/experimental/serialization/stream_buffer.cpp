#include "syntropy/experimental/serialization/streams/stream_buffer.h"

#include "syntropy/language/support.h"

namespace Syntropy
{
    /************************************************************************/
    /* STREAM BUFFER                                                        */
    /************************************************************************/

    Memory::ByteSpan StreamBuffer::Append(const Memory::ByteSpan& data)
    {
        auto data_size = Memory::Size(data);

        if (data_size > Memory::Bytes{ 0 })
        {
            if (auto size = append_size_ + data_size; size > GetCapacity())
            {
                size = Memory::ToBytes(Math::CeilTo<Int>(ToInt(size) * kGrowthFactor + kGrowthBias));                                               // Exponential growth to avoid continuous reallocations.

                Reserve(size);
            }

            auto append_position = append_size_;

            append_size_ += data_size;

            if (!transaction_)
            {
                size_ += data_size;                                                                                                                 // Commit immediately if there's no pending transaction.
            }

            return Write(append_position, data);                                                                                                    // Returned range is always empty.
        }

        return {};
    }

    Memory::RWByteSpan StreamBuffer::Consume(const Memory::RWByteSpan& data)
    {
        auto read_range = Read(Memory::Bytes{ 0 }, data);

        auto data_size = Memory::Size(read_range);

        base_pointer_ = GetAddress(data_size);

        consume_size_ -= data_size;

        if (!transaction_)
        {
            size_ -= data_size;
        }

        return read_range;
    }

    Memory::ByteSpan StreamBuffer::Write(Memory::Bytes position, const Memory::ByteSpan& data)
    {
        auto written_data = [this, position, &data]()
        {
            auto source = Front(data, ToInt(Math::Min(Memory::Size(data), size_ - position)));                                                      // Limit writable data to current buffer size.

            auto destination_begin = GetAddress(position);
            auto destination_end = GetAddress(position + Memory::Size(source));

            if (destination_begin < destination_end)
            {
                return Memory::Copy({ destination_begin, destination_end }, source);                                                                // Contiguous range.
            }
            else
            {
                return Memory::Scatter({ { destination_begin, End(buffer_.GetData()) }, { Begin(buffer_.GetData()), destination_end } }, source);   // Wrap-around range.
            }
        }();

        return { Begin(data) + written_data, End(data) };
    }

    Memory::RWByteSpan StreamBuffer::Read(Memory::Bytes position, const Memory::RWByteSpan& data) const
    {
        auto read_data = [this, position, &data]()
        {
            auto destination = Front(data, ToInt(Math::Min(Memory::Size(data), size_ - position)));                                                 // Limit readable data to current buffer size.

            auto source_begin = GetAddress(position);
            auto source_end = GetAddress(position + Memory::Size(destination));

            if (source_begin < source_end)
            {
                return Memory::Copy(destination, { source_begin, source_end });                                                                     // Contiguous range.
            }
            else
            {
                return Memory::Gather(destination, { { source_begin, static_cast<Memory::BytePtr>(End(buffer_.GetData())) }, { static_cast<Memory::BytePtr>(Begin(buffer_.GetData())), source_end } });     // Wrap-around range.
            }
        }();

        return { Begin(data), ToInt(read_data) };
    }

    void StreamBuffer::Realloc(Memory::Bytes capacity)
    {
        auto buffer = Memory::Buffer{ capacity, buffer_.GetAllocator() };

        if ((Memory::SizeOf(buffer_) > Memory::Bytes{ 0 }) && (capacity > Memory::Bytes{ 0 }))
        {
            auto head_pointer = GetAddress(size_);

            if (base_pointer_ < head_pointer)
            {
                Memory::Copy(buffer.GetData(), Memory::ByteSpan{ base_pointer_, head_pointer });                                                        // Contiguous range.
            }
            else
            {
                Memory::Gather(buffer.GetData(), { { base_pointer_, End(buffer_.GetData()) }, { Begin(buffer_.GetData()), head_pointer } });    // Wrap-around range.
            }

        }

        buffer_.Swap(buffer);

        base_pointer_ = Begin(buffer_.GetData());
    }

    Memory::RWBytePtr StreamBuffer::GetAddress(Memory::Bytes offset)
    {
        return const_cast<Memory::RWBytePtr>(ReadOnly(*this).GetAddress(offset));
    }

    Memory::BytePtr StreamBuffer::GetAddress(Memory::Bytes offset) const
    {
        offset = Memory::ToBytes(base_pointer_ + offset - Begin(buffer_.GetData()));    // Advance.

        offset = Memory::ToBytes(offset % Memory::Size(buffer_));                       // Wrap-around.

        return Begin(buffer_.GetData()) + offset;                                       // Offset in buffer-space.

        return nullptr;
    }

    void StreamBuffer::Commit(Memory::Bytes append_size, Memory::Bytes consume_size)
    {

    }

    void StreamBuffer::Rollback(Memory::Bytes append_size, Memory::Bytes consume_size)
    {

    }

}