
// ################################################################################

// @brief Contains lanaguage support definitions.
// @author Raffaele D. Facendola - April 2023

// ################################################################################

module;

#include <type_traits>

// ################################################################################

export module syntropy.language;

// ################################################################################

export namespace sy
{
    // ================================================================================
    // TRAITS
    // ================================================================================

    // If Type is a reference type, this is equal to the referenced type, otherwise this is equal to Type.
    template <typename Type>
    using RemoveReference = std::remove_reference_t<Type>;

    // ================================================================================
    // MOVE / FORWARD
    // ================================================================================

    // Indicates that rhs can be efficiently moved to another object.
    template <typename Type>
    constexpr RemoveReference<Type>&& Move(Type&& rhs);

    // Forwards lvalues as lvalues or rvalues depending on Type.
    template <typename Type>
    constexpr Type&& Forward(RemoveReference<Type>& rhs);

    // Forwards rvalues as rvalues and prohibits forwarding of rvalues as lvalues.
    template <typename Type>
    constexpr Type&& Forward(RemoveReference<Type>&& rhs);

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

// ================================================================================
// IMPLEMENTATION
// ================================================================================

template <typename Type>
constexpr sy::RemoveReference<Type>&& sy::Move(Type&& rhs)
{
    return static_cast<RemoveReference<Type>&&>(rhs);
}

template <typename Type>
constexpr Type&& sy::Forward(RemoveReference<Type>& rhs)
{
    return static_cast<Type&&>(rhs);
}

template <typename Type>
constexpr Type&& sy::Forward(RemoveReference<Type>&& rhs)
{
    return static_cast<Type&&>(rhs);
}

// ################################################################################
