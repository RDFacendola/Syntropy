
// ################################################################################

// @brief Contains base definitions for memory buffers.
// @author Raffaele D. Facendola - April 2023

// ################################################################################

export module syntropy.buffer;

import syntropy.language;
import syntropy.allocator;
import syntropy.memory;

// ################################################################################

export namespace sy
{
    // ================================================================================
    // BUFFER
    // ================================================================================

    // A buffer that is acquired during construction and released upon destruction.
    // Allocator bound to the buffer is never propagated.
    class Buffer : private NonCopyable<Buffer>
    {
    public:

        // Create an empty buffer on the current allocator.
        Buffer(Allocator& allocator = GetThreadAllocator());

        // Create a buffer on the current allocator.
        Buffer(Bytes size, Allocator& allocator = GetThreadAllocator());

        // Release the allocated buffer.
        ~Buffer();

        // Implicit conversion to a ByteSpan.
        operator ByteSpan() const;

        // Implicit conversion to a RWByteSpan.
        operator RWByteSpan() const;

        // Get the allocator the buffer was allocated on.
        Allocator& GetAllocator() const;

        // Get the buffer size.
        Bytes GetSize() const;

    private:

        // Allocator this buffer was allocated on.
        Allocator& _allocator;

        // Buffer data.
        RWByteSpan _data{};

    };

}

// ################################################################################
