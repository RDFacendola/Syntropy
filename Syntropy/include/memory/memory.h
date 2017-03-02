
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

        /// \brief Offset an address.
        /// \param address Address to perform the offset from.
        /// \param offset Offset with respect the address, in bytes.
        /// \return Returns the address offsetted by the specified amount.
        template <typename T>
        static constexpr T* Offset(T* address, int64_t offset);

        /// \brief Check whether an address is contained in a memory range.
        /// \param base Base address of the range.
        /// \param top First address not in the range.
        /// \param address Address to check.
        /// \return Returns true if the address is contained in the range [base; top), returns false otherwise.
        static constexpr bool IsContained(void* base, void* top, void* address);

        /// \brief Get the size of a memory range.
        /// \param base Base address of the range.
        /// \param top First address not in the range.
        /// \return Returns the size of the range [base; top), in bytes.
        static constexpr size_t GetSize(void* base, void* top);

        /// \brief Round an allocation size up to the next page size.
        /// \param size Size to round up.
        /// \return Returns the size extended such that is a multiple of the page size.
        static size_t CeilToPageSize(size_t size);

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

        // Virtual memory

        /// \brief Get the virtual memory page size.
        /// \return Returns the virtual memory page size, in bytes.
        static size_t GetPageSize();

        /// \brief Reserve and allocate a block of virtual memory.
        /// This method has the same effect as a Reserve() followed by a Commit().
        /// \param size Size of the block of memory to allocate, in bytes.
        /// \return Returns a pointer to the allocated memory block. If the allocation couldn't be fulfilled, returns nullptr.
        static void* Allocate(size_t size);

        /// \brief Release a reserved virtual memory block.
        /// \param address Address of the memory block to release.
        /// \return Returns true if the block could be released, returns false otherwise.
        /// \remarks The provided address must match the address returned by Reserve(), otherwise the behaviour is undefined.
        static bool Release(void* address);

        /// \brief Reserve an aligned block of virtual memory without allocating it.
        /// Use Commit() in order to access a reserved memory block.
        /// \param size Size of the block of memory to reserve, in bytes.
        /// \param alignment Alignment of the block of memory to reserve, in bytes.
        /// \return Returns the base address of the reserved virtual memory range.
        static void* Reserve(size_t size, size_t alignment);

        /// \brief Commit a reserved virtual memory block.
        /// This method allocates all the pages containing at least one byte in the range [address, address + size] and makes them accessible by the application.
        /// \param address Base address of the memory block to commit.
        /// \param size Size of the block to commit, in bytes.
        /// \return Returns true if the memory could be committed, returns false otherwise.
        /// \remarks address and size must refer to a memory region that was previously reserved via Reserve().
        static bool Commit(void* address, size_t size);

        /// \brief Decommit a virtual memory block.
        /// This method decommits all the pages containing at least one byte in the range [address, address + size].
        /// \param address Base address of the memory block to decommit.
        /// \param size Size of the block to decommit, in bytes.
        static bool Decommit(void* address, size_t size);

    private:

        /// \brief Header for a single reserved block.
        struct ReservedBlockHeader
        {
            void* base_address_;        ///< \brief Base address of the reserved block.
            void* block_address_;       ///< \brief Actual aligned address of the block.
            size_t size_;               ///< \brief Size of the memory block, in bytes.
            size_t extended_size_;      ///< \brief Size of the entire reserved memory block, in bytes. Includes both the block, the header and eventual padding.
            size_t alignment_;          ///< \brief Alignment of the memory block.
        };

        /// \brief Get a reserved block header starting from a block address.
        /// \param address Block address.
        /// \param page_size System page size.
        static ReservedBlockHeader* GetReservedBlockHeader(void* address, size_t page_size);

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

        /// \brief Default constructor.
        /// \param name Name of the allocator.
        Allocator(const HashedString& name);

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

        HashedString name_;                 ///< \brief Name of the allocator.

        diagnostics::Context context_;      ///< \brief Context associated to the allocator.

    };

    /// \brief Represents a range of contiguous virtual memory addresses.
    /// The range must be manually committed\decommitted before performing any access.
    /// \author Raffaele D. Facendola - December 2016
    class MemoryRange
    {
    public:

        /// \brief Create a new virtual memory range.
        /// \param capacity Capacity of the range, in bytes.
        MemoryRange(size_t capacity);

        /// \brief Create a new aligned virtual memory range.
        /// \param capacity Capacity of the range, in bytes.
        /// \param alignment Alignment of the range.
        MemoryRange(size_t capacity, size_t alignment);

        /// \brief No copy ctor.
        MemoryRange(const MemoryRange&) = delete;

        /// \brief Move ctor.
        MemoryRange(MemoryRange&& other);

        /// \brief No assignment operator.
        MemoryRange& operator=(const MemoryRange&) = delete;

        /// \brief Default destructor.
        ~MemoryRange();

        /// \brief Dereferencing operator. Access the base address of the range.
        /// \return Returns the base address of the range.
        void* operator*() const;

        /// \brief Access an element in the range.
        /// \param offset Offset with respect to the first element of the range.
        /// \return Returns a pointer to offset-bytes after the base of the range.
        void* operator[](size_t offset) const;

        /// \brief Get the total capacity of the memory range, in bytes.
        /// \return Returns the total capacity of the memory range, in bytes.
        size_t GetCapacity() const;

        /// \brief Check whether a memory sub-range belongs entirely to this memory range.
        /// \param address Base address of the sub-range to check.
        /// \param size Size of the sub-range, in bytes.
        /// \return Returns true if the sub-range [address, address+size) is contained inside this memory range, return false otherwise.
        bool Contains(void* address, size_t size) const;

        /// \brief Commit a memory block within this memory range.
        /// This method allocates all the memory pages containing at least one byte in the range [address, address + size].
        /// \param address Address of the memory block to allocate.
        /// \param size Size of the block to allocate, in bytes.
        void Commit(void* address, size_t size);

        /// \brief Decommit a memory block.
        /// This method free all the pages containing at least one byte in the range [address, address + size].
        /// \param address Base address of the memory block to free.
        void Decommit(void* address, size_t size);

    private:

        void* base_;            ///< \brief First address in the memory range. Owning pointer.

        size_t capacity_;       ///< \brief Capacity of the range, in bytes.

    };

    /// \brief Represents a raw memory buffer.
    /// \author Raffaele D. Facendola - February 2017
    class MemoryBuffer
    {
    public:

        /// \brief Create a new empty buffer.
        MemoryBuffer();

        /// \brief Create a new buffer.
        /// \param base Base address of the buffer.
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

        /// \brief Swap the content of this buffer with another one.
        void Swap(MemoryBuffer& other) noexcept;

    private:

        void* buffer_;          ///< \brief First address in the buffer. Owning pointer.

        size_t size_;           ///< \brief Size of the buffer, in bytes.

        Allocator* allocator_;  ///< \brief Allocator used to allocate\deallocate memory. Non-owning pointer.

    };

}

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

namespace syntropy
{

    /************************************************************************/
    /* MEMORY                                                               */
    /************************************************************************/

    template <typename T>
    constexpr T* Memory::Offset(T* address, int64_t offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<int8_t*>(address) + offset);
    }

    inline constexpr bool Memory::IsContained(void* base, void* top, void* address)
    {
        return reinterpret_cast<uintptr_t>(base) <= reinterpret_cast<uintptr_t>(address) &&
               reinterpret_cast<uintptr_t>(address) < reinterpret_cast<uintptr_t>(top);
    }
    
    inline constexpr size_t Memory::GetSize(void* base, void* top)
    {
        return reinterpret_cast<uintptr_t>(top) - reinterpret_cast<uintptr_t>(base);
    }

    template <typename T>
    constexpr T* Memory::Align(T* address, size_t alignment)
    {
        return reinterpret_cast<T*>(Math::Ceil(reinterpret_cast<uintptr_t>(address), alignment));
    }

    template <typename T>
    static constexpr T* Memory::AlignDown(T* address, size_t alignment)
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