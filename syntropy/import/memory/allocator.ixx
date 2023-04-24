
// ################################################################################

// @brief Contains base definitions for allocators.
// @author Raffaele D. Facendola - April 2023

// ################################################################################

export module syntropy.allocator;

import syntropy.language;
import syntropy.types;
import syntropy.memory;

// ################################################################################

export namespace sy
{
    // ================================================================================
    // ALLOCATOR
    // ================================================================================

    // Base class for allocators.
    class Allocator : private NonCopyable<Allocator>
    {
    public:

        // Default constructor.
        Allocator() = default;

        // Virtual destructor.
        virtual ~Allocator() = default;

        // Allocate a memory block. If memory could not be allocated, returns an empty block.
        [[nodiscard]] virtual RWByteSpan Allocate(Bytes size, Alignment alignment) = 0;

        // Deallocate a memory block.
        // @remarks Calling this method with a memory block that was not returned by a previous call to ::Allocate
        //          results in undefined behaviour.
        virtual void Deallocate(const RWByteSpan& block, Alignment alignment) = 0;

    };

    // Get the active allocator in this thread.
    Allocator& GetThreadAllocator();

    // Set the active allocator in this thread.
    void SetThreadAllocator(Allocator& allocator);

    // ================================================================================
    // SYSTEM ALLOCATOR
    // ================================================================================

    // Singleton allocator that wraps systems ::new/::delete operators.
    class SystemAllocator : public Allocator
    {

        friend SystemAllocator& GetSystemAllocator();

    public:

        [[nodiscard]] virtual RWByteSpan Allocate(Bytes size, Alignment alignment) override;

        virtual void Deallocate(const RWByteSpan& block, Alignment alignment) override;

    private:

        // Private default constructor.
        SystemAllocator() = default;

    };

    // Get the system allocator singleton.
    SystemAllocator& GetSystemAllocator();

    // ================================================================================
    // ALLOCATOR GUARD
    // ================================================================================

    // RAII guard used to activate an allocator in the scope and restore the previous one upon destruction.
    class AllocatorGuard : private NonCopyable<AllocatorGuard>
    {
    public:

        // Activate a new allocator in the scope.
        AllocatorGuard(Allocator& allocator);

        // Restore the previous allocator.
        ~AllocatorGuard();

    private:

        // Allocator to restore upon guard destruction.
        Allocator& _allocator;

    };

}

// ################################################################################
