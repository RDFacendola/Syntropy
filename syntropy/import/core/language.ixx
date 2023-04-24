
// ################################################################################

// @brief Contains lanaguage support definitions.
// @author Raffaele D. Facendola - April 2023

// ################################################################################

export module syntropy.language;

// ################################################################################

export namespace sy
{
    // ================================================================================
    // NON-COPYABLE
    // ================================================================================

    // Mixin used to prevent an object from being copied or moved.
    // Usage: class MyClass : private NonCopyable<MyClass>
    template <typename Type>
    class NonCopyable
    {
    public:

        // Default constructor.
        NonCopyable() = default;

        // No copy-constructor.
        NonCopyable(const NonCopyable&) = delete;

        // No move-constructor.
        NonCopyable(NonCopyable&&) = delete;

        // Default destructor.
        ~NonCopyable() = default;

        // No copy-assignment operator.
        NonCopyable& operator=(const NonCopyable&) = delete;

        // No move-assignment operator.
        NonCopyable& operator=(NonCopyable&&) = delete;

    };

}

// ################################################################################
