
/// \file polymorphic_deleter.h
/// \brief This header is part of the Syntropy allocators module. It contains definitions of polymorphic deleters.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/utility.h"
#include "syntropy/core/types.h"
#include "syntropy/memory/memory.h"
#include "syntropy/memory/allocator.h"

namespace syntropy
{
    /************************************************************************/
    /* POLYMORPHIC DELETER                                                  */
    /************************************************************************/

    /// \brief Deleter used to destroy objects allocated via a Allocator.
    /// The underlying type is erased to promote propagation and assignment with smart pointers.
    /// \author Raffaele D. Facendola - May 2020.
    class PolymorphicDeleter
    {
    public:

        /// \brief Default constructor with implicit default memory resource.
        PolymorphicDeleter() = default;

        /// \brief Create a new polymorphic deleter with explicit memory resource.
        template <typename TType>
        PolymorphicDeleter(TypeT<TType>, Allocator& memory_resource) noexcept;

        /// \brief Destroy an object allocated on the underlying memory resource.
        void operator()(RWTypelessPtr object);

    private:

        /// \brief Type of a function used to destroy objects.
        using TDestructor = void(*)(Allocator&, RWTypelessPtr);

        /// \brief Function used to destroy strongly-typed objects.
        /// If the provided object's type is not equal to TType the behavior of this method is undefined.
        template <typename TType>
        static void Destroy(Allocator& memory_resource, RWTypelessPtr object);

        /// \brief Destructor.
        TDestructor destructor_{ nullptr };

        /// \brief Underlying memory resource.
        Pointer<Allocator> memory_resource_ = &Memory::GetAllocator();

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new deleter from a memory resource.
    /// The returned deleter can deallocate any object allocated via the provided memory resource.
    template <typename TType>
    PolymorphicDeleter MakePolymorphicDeleter(Allocator& memory_resource);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // PolymorphicDeleter<TType>.

    template <typename TType>
    inline PolymorphicDeleter::PolymorphicDeleter(TypeT<TType>, Allocator& memory_resource) noexcept
        : destructor_(&Destroy<TType>)
        , memory_resource_(&memory_resource)
    {

    }

    inline void PolymorphicDeleter::operator()(RWTypelessPtr object)
    {
        destructor_(*memory_resource_, object);
    }

    template <typename TType>
    inline void PolymorphicDeleter::Destroy(Allocator& memory_resource, RWTypelessPtr object)
    {
        if (object)
        {
            auto object_ptr = ToPointer<TType>(object);

            auto storage = Memory::RWBytesOf(*object_ptr);          // Memory must be read before destroying the object in order to account for proper dynamic type.

            DestroyAt(object_ptr);

            memory_resource.Deallocate(storage, Memory::AlignmentOf<TType>());
        }
    }

    // Non-member functions.

    template <typename TType>
    inline PolymorphicDeleter MakePolymorphicDeleter(Allocator& memory_resource)
    {
        return PolymorphicDeleter(kType<TType>, memory_resource);
    }

}
