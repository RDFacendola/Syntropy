#include "syntropy/core/byte_string.h"

namespace syntropy
{
    /************************************************************************/
    /* BYTE STRING                                                          */
    /************************************************************************/

    void ByteString::Write(Int position, const ConstMemoryRange& data)
    {
        auto capacity = data.GetSize() + Bytes{ position };                             // Capacity needed to perform the copy.

        size_ = Math::Max(size_, capacity);

        if (capacity > GetCapacity())
        {
            capacity = ToBytes(ToInt((*capacity) * kGrowthFactor + kGrowthBias));       // Extra space to prevent frequent reallocations.

            Realloc(capacity);
        }

        auto destination = MemoryRange{ buffer_.Begin() + Bytes{ position }, data.GetSize() };

        Memory::Copy(destination, data);
    }

    MemoryRange ByteString::Read(Int position, const MemoryRange& data)
    {
        auto source = ConstMemoryRange{ buffer_.Begin() + Bytes{position}, data.GetSize() };

        source = Intersection(source, buffer_.GetConstData());

        auto bytes = Memory::Copy(data, source);

        auto zero = MemoryRange{ data }.PopFront(bytes);

        Memory::Zero(zero);

        return { data.Begin(), bytes };
    }

    void ByteString::Realloc(Bytes size)
    {
        if (size != buffer_.GetSize())
        {
            auto buffer = MemoryBuffer(size, buffer_.GetMemoryResource());

            Memory::Copy(buffer.GetData(), buffer_.GetData());

            buffer_.Swap(buffer);
        }
    }

}