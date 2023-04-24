
// ################################################################################

// @brief Contains definition for unique pointers.
// @author Raffaele D. Facendola - April 2023

// ################################################################################

export module syntropy.unique;

import syntropy.types;

// ################################################################################

export namespace sy
{
    // ================================================================================
    // UNIQUE
    // ================================================================================

    // A pointer that holds exclusive ownership of an object and disposes it when going
    // out of scope.
    //template <typename Type>
    //class Unique
    //{
    //public:

    //    // Default constructor.
    //    constexpr Unique() = default;

    //    // Create an empty pointer.
    //    constexpr Unique(Null);

    //    // No copy-constructor.
    //    constexpr Unique(const Unique&) = delete;

    //    // Move converting constructor.
    //    template <typename TypeU>
    //    constexpr Unique(Unique<TypeU>&& rhs);

    //    // Destroy the underlying object.
    //    constexpr ~Unique();

    //    // No copy-assignment.
    //    constexpr operator=(const Unique&) = delete;

    //    // Move assignment.
    //    constexpr Unique& operator=(Unique&& rhs);

    //    // Check whether the pointer is valid.
    //    constexpr explicit operator Bool() const;

    //    // Access the underlying object.
    //    constexpr Type& operator*() const;

    //    // Access the underlying object.
    //    constexpr Type* operator->() const;

    //private:

    //    // Allocator this buffer was allocated on.
    //    Allocator& _allocator;

    //    // Buffer data.
    //    RWByteSpan _data{};

    //    // Buffer alignment.
    //    Alignment _alignment{ 0 };

    //};

}

// ################################################################################

// ================================================================================
// IMPLEMENTATION
// ================================================================================

// ################################################################################
