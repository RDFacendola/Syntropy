#include "syntropy/experimental/serialization/streams/stream_buffer.h"

#include "syntropy/language/utility.h"

namespace syntropy
{
    /************************************************************************/
    /* STREAM BUFFER                                                        */
    /************************************************************************/

    ConstMemoryRange StreamBuffer::Append(const ConstMemoryRange& data)
    {
        auto data_size = data.GetSize();

        if (data_size > Bytes{ 0 })
        {
            if (auto size = append_size_ + data_size; size > GetCapacity())
            {
                size = ToBytes(Math::CeilTo<Int>((*size) * kGrowthFactor + kGrowthBias));                                                       // Exponential growth to avoid continuous reallocations.

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

    MemoryRange StreamBuffer::Consume(const MemoryRange& data)
    {
        auto read_range = Read(Bytes{ 0 }, data);

        auto data_size = read_range.GetSize();

        base_pointer_ = GetAddress(data_size);

        consume_size_ -= data_size;

        if (!transaction_)
        {
            size_ -= data_size;
        }

        return read_range;
    }

    ConstMemoryRange StreamBuffer::Write(Bytes position, const ConstMemoryRange& data)
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

    MemoryRange StreamBuffer::Read(Bytes position, const MemoryRange& data) const
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

    void StreamBuffer::Realloc(Bytes capacity)
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

    MemoryAddress StreamBuffer::GetAddress(Bytes offset)
    {
        auto address = AsConst(*this).GetAddress(offset).As<void>();

        return const_cast<void*>(address);
    }

    ConstMemoryAddress StreamBuffer::GetAddress(Bytes offset) const
    {
        offset = (base_pointer_ + offset - buffer_.Begin());        // Advance.

        offset = ToBytes(offset % buffer_.GetSize());               // Wrap-around.

        return buffer_.Begin() + offset;                            // Offset in buffer-space.

        return nullptr;
    }

    void StreamBuffer::Commit(Bytes append_size, Bytes consume_size)
    {

    }

    void StreamBuffer::Rollback(Bytes append_size, Bytes consume_size)
    {

    }

}