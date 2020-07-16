#include "syntropy/experimental/serialization/streams/stream_buffer.h"

#include "syntropy/language/utility.h"

namespace syntropy
{
    /************************************************************************/
    /* STREAM BUFFER                                                        */
    /************************************************************************/

    ByteSpan StreamBuffer::Append(const ByteSpan& data)
    {
        auto data_size = Size(data);

        if (data_size > Bytes{ 0 })
        {
            if (auto size = append_size_ + data_size; size > GetCapacity())
            {
                size = ToBytes(Math::CeilTo<Int>(ToInt(size) * kGrowthFactor + kGrowthBias));                                                   // Exponential growth to avoid continuous reallocations.

                Reserve(size);
            }

            auto append_position = append_size_;

            append_size_ += data_size;

            if (!transaction_)
            {
                size_ += data_size;                                                                                                             // Commit immediately if there's no pending transaction.
            }

            return Write(append_position, data);                                                                                                // Returned range is always empty.
        }

        return {};
    }

    RWByteSpan StreamBuffer::Consume(const RWByteSpan& data)
    {
        auto read_range = Read(Bytes{ 0 }, data);

        auto data_size = Size(read_range);

        base_pointer_ = GetAddress(data_size);

        consume_size_ -= data_size;

        if (!transaction_)
        {
            size_ -= data_size;
        }

        return read_range;
    }

    ByteSpan StreamBuffer::Write(Bytes position, const ByteSpan& data)
    {
        auto written_data = [this, position, &data]()
        {
            auto source = First(data, ToInt(Math::Min(Size(data), size_ - position)));                                                          // Limit writable data to current buffer size.

            auto destination_begin = GetAddress(position);
            auto destination_end = GetAddress(position + Size(source));

            if (destination_begin < destination_end)
            {
                return Memory::Copy({ destination_begin, destination_end }, source);                                                            // Contiguous range.
            }
            else
            {
                return Memory::Scatter({ { destination_begin, End(buffer_.GetData()) }, { Begin(buffer_.GetData()), destination_end } }, source);                 // Wrap-around range.
            }
        }();

        return { Begin(data) + written_data, End(data) };
    }

    RWByteSpan StreamBuffer::Read(Bytes position, const RWByteSpan& data) const
    {
        auto read_data = [this, position, &data]()
        {
            auto destination = First(data, ToInt(Math::Min(Size(data), size_ - position)));                                                 // Limit readable data to current buffer size.

            auto source_begin = GetAddress(position);
            auto source_end = GetAddress(position + Size(destination));

            if (source_begin < source_end)
            {
                return Memory::Copy(destination, { source_begin, source_end });                                                                 // Contiguous range.
            }
            else
            {
                return Memory::Gather(destination, { { source_begin, static_cast<BytePtr>(End(buffer_.GetData())) }, { static_cast<BytePtr>(Begin(buffer_.GetData())), source_end } });     // Wrap-around range.
            }
        }();

        return { Begin(data), ToInt(read_data) };
    }

    void StreamBuffer::Realloc(Bytes capacity)
    {
        auto buffer = MemoryBuffer{ capacity, buffer_.GetMemoryResource() };

        if ((buffer_.GetSize() > Bytes{ 0 }) && (capacity > Bytes{ 0 }))
        {
            auto head_pointer = GetAddress(size_);

            if (base_pointer_ < head_pointer)
            {
                Memory::Copy(buffer.GetData(), ByteSpan{ base_pointer_, head_pointer });                                                        // Contiguous range.
            }
            else
            {
                Memory::Gather(buffer.GetData(), { { base_pointer_, End(buffer_.GetData()) }, { Begin(buffer_.GetData()), head_pointer } });    // Wrap-around range.
            }

        }

        buffer_.Swap(buffer);

        base_pointer_ = Begin(buffer_.GetData());
    }

    RWBytePtr StreamBuffer::GetAddress(Bytes offset)
    {
        return const_cast<RWBytePtr>(ReadOnly(*this).GetAddress(offset));
    }

    BytePtr StreamBuffer::GetAddress(Bytes offset) const
    {
        offset = ToBytes(base_pointer_ + offset - Begin(buffer_.GetData()));    // Advance.

        offset = ToBytes(offset % buffer_.GetSize());                           // Wrap-around.

        return Begin(buffer_.GetData()) + offset;                               // Offset in buffer-space.

        return nullptr;
    }

    void StreamBuffer::Commit(Bytes append_size, Bytes consume_size)
    {

    }

    void StreamBuffer::Rollback(Bytes append_size, Bytes consume_size)
    {

    }

}