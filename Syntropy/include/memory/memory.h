
/// \file memory.h
/// \brief TODO: Add brief description here
///
/// IMPORTANT: Array allocation\deallocation is not supported. 
/// Placement delete for arrays cannot work with the standard syntax new[] since there's no way of interpreting bookkeeping data (or *where* it is stored)
/// auto p = new (Arena) Foo[5];
/// delete[] p;                         // ERROR: Arena is not used.
/// operator delete[](p, Arena);        // ERROR: This will only deallocate the memory without calling the dtor of Foo. Furthermore we don't know where the bookkeeping data are stored.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <cstdint>

#include "diagnostics/diagnostics.h"
#include "containers/hashed_string.h"
#include "math/math.h"

/// \brief Instantiate a new object via custom allocator.
/// \usage auto foo = SYNTROPY_NEW(allocator) Foo();
#define SYNTROPY_NEW(allocator) \
    new (allocator, SYNTROPY_HERE)

/// \brief Delete an object created via custom allocator.
/// \usage SYNTROPY_DELETE(allocator, pointer);
#define SYNTROPY_DELETE(allocator, ptr) \
    syntropy::Allocator::Delete(ptr, allocator, SYNTROPY_HERE);

/// \brief Allocate a new buffer via custom allocator.
/// \usage void* buffer = SYNTROPY_ALLOC(allocator, size);
#define SYNTROPY_ALLOC(allocator, size) \
    operator new(size, allocator, SYNTROPY_HERE)

/// \brief Free a buffer allocated via custom allocator.
/// \usage SYNTROPY_FREE(allocator, buffer);
#define SYNTROPY_FREE(allocator, ptr) \
    operator delete(ptr, allocator, SYNTROPY_HERE)

namespace syntropy
{

    /// \brief Log context for the syntropy memory system.
    extern const diagnostics::Context MemoryCtx;

    /// \brief Wraps the low-level calls used to handle virtual memory allocation.
    /// \author Raffaele D. Facendola - December 2016
    class Memory
    {
    public:

        // Memory manipulation

        /// \brief Add an offset to an address.
        /// \param address Address to offset.
        /// \param offset Offset to add, in bytes.
        /// \return Returns the address moved forward by the specified amount of bytes.
        template <typename T>
        static constexpr T* AddOffset(T* address, size_t offset);

        /// \brief Subtract and offset from an address.
        /// \param address Address to offset.
        /// \param offset Offset to subtract, in bytes.
        /// \return Returns the address moved backward by the specified amount of bytes.
        template <typename T>
        static constexpr T* SubOffset(T* address, size_t offset);

        /// \brief Get the distance of two addresses, in bytes.
        /// \param first First address.
        /// \param second Second address.
        /// \return Returns the signed distance among first and second, in bytes. The result is negative if second is less than first, positive otherwise.
        static constexpr ptrdiff_t GetDistance(const void* first, const void* second);

        // Memory alignment

        /// \brief Align an address.
        /// \param address Address to align
        /// \param alignment Alignment.
        /// \return Returns the address aligned to the given alignment boundary.
        /// \remarks This method aligns by adding a padding value to the base address.
        template <typename T>
        static constexpr T* Align(T* address, size_t alignment);

        /// \brief Align an address.
        /// \param address Address to align
        /// \param alignment Alignment.
        /// \return Returns the address aligned down to the given alignment boundary.
        /// \remarks This method aligns by subtracting a padding value to the base address.
        template <typename T>
        static constexpr T* AlignDown(T* address, size_t alignment);

        /// \brief Check whether an address is aligned to a particular size.
        /// \param address Address to check.
        /// \param alignment Alignment to test against.
        /// \return Returns true if address is aligned to the provided alignment, returns false otherwise.
        static constexpr bool IsAlignedTo(void* address, size_t alignment);

    };

    /// \brief Incapsulates memory debug functionalities.
    /// \author Raffaele D. Facendola - March 2017
    class MemoryDebug
    {
    public:

        /// \brief Mark a memory region as uninitialized.
        /// This method does nothing on release.
        /// \param begin First address in the range.
        /// \param end One past the last address in the range.
        static void MarkUninitialized(void* begin, void* end);

        /// \brief Mark a memory region as free.
        /// This method does nothing on release.
        /// \param begin First address in the range.
        /// \param end One past the last address in the range.
        static void MarkFree(void* begin, void* end);

    private:

        /// \brief Pattern used to identify an uninitialized memory block (ie: allocated but not yet written).
        static constexpr int8_t kUninitializedMemoryPattern = 0x5C;

        /// \brief Pattern used to identify a free memory block (ie: a block that has not been yet allocated).
        static constexpr int8_t kFreeMemoryPattern = 0x5F;

    };

    /// \brief Base interface for allocators.
    /// \author Raffaele D. Facendola - February 2017
    class Allocator
    {
    public:

        /// \brief Delete an object allocated via a custom allocator.
        /// \param ptr Pointer to the object to delete.
        /// \param allocator Allocator used to allocate the object.
        template <typename T>
        static void Delete(T* ptr, Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace);

        /// \brief Get an allocator by name.
        /// \return Returns the allocator with a matching name, if any. If no such allocator could be found returns nullptr.
        static Allocator* GetAllocatorByName(const HashedString& name);

        /// Create a new anonymous allocator.
        Allocator();

        /// \brief Create a new named allocator.
        /// \param name Name of the allocator. This name should be unique.
        Allocator(const HashedString& name);

        /// \brief Virtual destructor.
        virtual ~Allocator();

        /// \brief Allocate a new memory block.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block.
        virtual void* Allocate(size_t size) = 0;

        /// \brief Allocate a new aligned memory block.
        /// \param size Size of the memory block to allocate, in bytes.
        /// \param alignment Alignment of the allocated block. Must be a multiple of the minimum allocation size.
        /// \return Returns a pointer to the allocated memory block.
        virtual void* Allocate(size_t size, size_t alignment) = 0;

        /// \brief Free a memory block.
        /// The caller must ensure that the block belongs to the allocator otherwise the behaviour is undefined.
        /// \param block Pointer to the block to free.
        virtual void Free(void* block) = 0;

        /// \brief Check whether the allocator owns the provided block.
        /// \param block Address to check.
        ///\ return Returns true if block was belongs to this allocator, returns false otherwise.
        virtual bool Belongs(void* block) const = 0;

        /// \brief Get the biggest allocation that can be performed by this allocator.
        virtual size_t GetMaxAllocationSize() const = 0;

        /// \brief Get a symbolic name for the allocator.
        /// \return Returns a symbolic name for the allocator.
        const HashedString& GetName() const;

        /// \brief Get the context associated to this allocator instance.
        operator diagnostics::Context() const;

    private:

        HashedString name_;                             ///< \brief Name of the allocator.

        diagnostics::Context context_;                  ///< \brief Context associated to the allocator.

        static std::vector<Allocator*> allocators_;     ///< \brief List of registered allocators.
    };

    /// \brief Represents a range of contiguous memory addresses.
    /// \author Raffaele D. Facendola - December 2016
    class MemoryRange
    {
    public:

        /// \brief Create an empty memory range.
        MemoryRange();

        /// \brief Create a memory range.
        /// \param base First address in the range.
        /// \param top One past the last address in the range.
        MemoryRange(void* base, void* top);

        /// \brief Create a memory range.
        /// \param base First address in the range.
        /// \param size Size of the range.
        MemoryRange(void* base, size_t size);

        /// \brief Create a memory range which is the aligned version of another range.
        /// This range is guaranteed to be contained inside the original range.
        /// \param other Range to copy.
        /// \param alignment Alignment of this range.
        MemoryRange(const MemoryRange& other, size_t alignment);

        /// \brief Dereferencing operator. Access the base address of the range.
        /// \return Returns the base address of the range.
        void* operator*() const;

        /// \brief Access an element in the range.
        /// \param offset Offset with respect to the first element of the range.
        /// \return Returns a pointer to offset-bytes after the base of the range.
        void* operator[](size_t offset) const;

        /// \brief Get the address past the end of this range,
        /// \return Returns the address past the end of this range.
        void* GetTop() const;

        /// \brief Get the size of the range, in bytes.
        /// \return Returns the total capacity of the memory range, in bytes.
        size_t GetSize() const;

        /// \brief Check whether a memory range is contained entirely inside this range.
        /// \param memory_range Memory range to check.
        /// \return Returns true if memory_range is contained inside this memory range, returns false otherwise.
        bool Contains(const MemoryRange& memory_range) const;

        /// \brief Check whether an address falls within this memory range.
        /// \param address Address to check.
        /// \return Returns true if address is contained inside this memory range, returns false otherwise.
        bool Contains(void* address) const;

    private:

        void* base_;            ///< \brief First address in the memory range.

        void* top_;             ///< \brief One past the last address in the memory range.

    };

    /// \brief Represents a raw memory buffer.
    /// \author Raffaele D. Facendola - February 2017
    class MemoryBuffer
    {
    public:

        /// \brief Create a new empty buffer.
        MemoryBuffer();

        /// \brief Create a new buffer.
        /// \param size Size of the buffer, in bytes.
        /// \param allocator Allocator used to allocate the memory.
        MemoryBuffer(size_t size, Allocator& allocator);

        /// \brief Copy constructor.
        /// Copy the content of another buffer to this one.
        /// \param other Buffer to copy.
        MemoryBuffer(const MemoryBuffer& other);

        /// \brief Move constructor.
        /// Assign the memory buffer of another instance to this one.
        /// \param other Buffer to move.
        MemoryBuffer(MemoryBuffer&& other);

        /// \brief Destructor.
        ~MemoryBuffer();

        /// \brief Unified assignment operator.
        MemoryBuffer& operator=(MemoryBuffer other);

        /// \brief Dereferencing operator. Access the base address of the buffer.
        /// \return Returns the base address of the buffer.
        void* operator*() const;

        /// \brief Access an element in the buffer.
        /// \param offset Offset with respect to the first element of the buffer.
        /// \return Returns a pointer to the element (buffer+offset).
        void* operator[](size_t offset) const;

        /// \brief Get the size of the buffer, in bytes.
        /// \return Returns the size of the buffer, in bytes.
        size_t GetSize() const;

        /// \brief Get the buffer memory range.
        operator MemoryRange() const;

        /// \brief Swap the content of this buffer with another one.
        void Swap(MemoryBuffer& other) noexcept;

    private:

        MemoryRange range_;     ///< \brief Buffer memory range.

        Allocator* allocator_;  ///< \brief Allocator used to allocate\deallocate memory. Non-owning pointer.
    };

}

// Allocation

/// \brief New operator overload for custom allocators.
void* operator new (std::size_t size, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace);

/// \brief Delete expression for custom allocators.
/// Used to *deallocate* object allocated via custom allocators.
void operator delete (void* ptr, syntropy::Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace);

namespace std
{
    /// \brief Swap specialization for memory buffers.
    template<>
    void swap(syntropy::MemoryBuffer& first, syntropy::MemoryBuffer& second);
}

// Implementation

namespace syntropy
{

    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    template <typename T>
    constexpr T* Memory::AddOffset(T* address, size_t offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<int8_t*>(address) + offset);
    }

    template <typename T>
    constexpr T* Memory::SubOffset(T* address, size_t offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<int8_t*>(address) - offset);
    }

    constexpr ptrdiff_t Memory::GetDistance(const void* first, const void* second)
    {
        return reinterpret_cast<intptr_t>(second) - reinterpret_cast<intptr_t>(first);
    }

    template <typename T>
    constexpr T* Memory::Align(T* address, size_t alignment)
    {
        return reinterpret_cast<T*>(Math::Ceil(reinterpret_cast<uintptr_t>(address), alignment));
    }

    template <typename T>
    constexpr T* Memory::AlignDown(T* address, size_t alignment)
    {
        return reinterpret_cast<T*>(Math::Floor(reinterpret_cast<uintptr_t>(address), alignment));
    }

    constexpr bool Memory::IsAlignedTo(void* address, size_t alignment)
    {
        return reinterpret_cast<uintptr_t>(address) % alignment == 0;
    }

    /************************************************************************/
    /* ALLOCATOR                                                            */
    /************************************************************************/

    template <typename T>
    void Allocator::Delete(T* ptr, Allocator& allocator, const syntropy::diagnostics::StackTrace& stack_trace)
    {
        ptr->~T();
        operator delete(ptr, allocator, stack_trace);
    }

}

