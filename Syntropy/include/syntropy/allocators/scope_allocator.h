
/// \file scope_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains scope-based allocators used to automatically destruct objects on rewindable memory resources.
///
/// \author Raffaele D. Facendola - March 2017

#pragma once

#include <utility>

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/memory_address.h"

#include "syntropy/diagnostics/assert.h"

namespace syntropy
{
    /************************************************************************/
    /* SCOPE ALLOCATOR <TMEMORY RESOURCE>                                   */
    /************************************************************************/

    /// \brief Utility allocator that sits on top of a rewindable memory resource and uses RAII paradigm to automatically destruct objects when going out of scope.
    /// \tparam TMemoryResource Type of the underlying memory resource. The memory resource must be rewindable by means of SaveState()\RestoreState() method pairs.
    /// \author Raffaele D. Facendola - January 2017
    /// \remarks Based on https://media.contentapi.ea.com/content/dam/eacom/frostbite/files/scopestacks-public.pdf
    template <typename TMemoryResource>
    class ScopeAllocator
    {

        /// \brief Type of the memory resource state.
        using TState = decltype(std::declval<TMemoryResource>().SaveState());

    public:

        /// \brief Create a new scope allocator.
        /// \param memory_resource Underlying memory resource.
        ScopeAllocator(TMemoryResource& memory_resource);

        /// \brief Default destructor.
        /// Destroy all the objects that were allocated from this allocator and restore the underlying memory resource status.
        ~ScopeAllocator();

        /// \brief Create a new object of type TObject.
        template <typename TObject, typename... TArguments>
        TObject* New(TArguments&&... arguments);

        /// \brief Create a new aligned object of type TObject.
        template <typename TObject, typename... TArguments>
        TObject* AlignedNew(Alignment alignment, TArguments&&... arguments);

    private:

        /// \brief Finalizer object used to destroy non-trivially-destructible objects.
        struct Finalizer
        {
            /// \brief Type of a finalizer.
            using TFinalizer = void(*)(void*);

            /// \brief Destroy the object after this finalizer.
            void operator()();

            /// \brief Get the object bound to this finalizer.
            void* GetObject();

            /// \brief Concrete object destructor.
            TFinalizer destructor_;

            /// \brief Next finalizer.
            Finalizer* next_{ nullptr };
        };

        /// \brief Destroy an object of type TObject.
        /// \param instance Pointer to the instance to destroy. Must be of type TObject, otherwise the behavior of this method is undefined..
        template <typename TObject>
        static void Destructor(void* instance);

        /// \brief Allocate an object of type TObject along with its finalizer (if required).
        template <typename TObject>
        MemoryAddress AllocateObjectWithFinalizer();

        /// \brief Allocate an aligned object of type TObject along with its finalizer (if required).
        template <typename TObject>
        MemoryAddress AllocateObjectWithFinalizer(Alignment alignment);

        /// \brief Construct the finalizer for the type TObject.
        template <typename TObject>
        void ConstructFinalizer(Finalizer& finalizer);

        /// \brief Underlying memory resource.
        TMemoryResource& memory_resource_;

        /// \brief First finalizer.
        Finalizer* finalizers_{ nullptr };

        /// \brief Memory resource state to restore.
        TState state_;
    };

    /// \brief Utility method used to make a new scope allocator.
    template <typename TMemoryResource>
    ScopeAllocator<TMemoryResource> MakeScopeAllocator(TMemoryResource& memory_resource);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ScopeAllocator<TMemoryResource>.

    template <typename TMemoryResource>
    inline ScopeAllocator<TMemoryResource>::ScopeAllocator(TMemoryResource& memory_resource)
        : memory_resource_(memory_resource)
        , finalizers_(nullptr)
        , state_(memory_resource.SaveState())
    {

    }

    template <typename TMemoryResource>
    inline ScopeAllocator<TMemoryResource>::~ScopeAllocator()
    {
        for(;finalizers_; finalizers_ = finalizers_->next_)
        {
            (*finalizers_)();
        }

        memory_resource_.RestoreState(state_);
    }

    template <typename TMemoryResource>
    template <typename TObject, typename... TArguments>
    inline TObject* ScopeAllocator<TMemoryResource>::New(TArguments&&... arguments)
    {
        auto storage = AllocateObjectWithFinalizer<TObject>();

        return new (*storage) TObject(std::forward<TArguments>(arguments)...);
    }

    template <typename TMemoryResource>
    template <typename TObject, typename... TArguments>
    inline  TObject* ScopeAllocator<TMemoryResource>::AlignedNew(Alignment alignment, TArguments&&... arguments)
    {
        auto storage = AllocateObjectWithFinalizer<TObject>(alignment);

        return new (*storage) TObject(std::forward<TArguments>(arguments)...);
    }

    template <typename TMemoryResource>
    template <typename TObject>
    inline void ScopeAllocator<TMemoryResource>::Destructor(void* instance)
    {
        static_cast<TObject*>(instance)->~TObject();
    }

    template <typename TMemoryResource>
    template <typename TObject>
    MemoryAddress ScopeAllocator<TMemoryResource>::AllocateObjectWithFinalizer()
    {
        auto object_size = BytesOf<TObject>();

        if constexpr(std::is_trivially_destructible_v<TObject>)
        {
            return memory_resource_.Allocate(object_size).Begin();
        }
        else
        {
            auto finalizer_size = BytesOf<Finalizer>();

            auto finalizer = memory_resource_.Allocate(object_size + finalizer_size).Begin().As<Finalizer>();

            ConstructFinalizer<TObject>(*finalizer);

            return finalizer->GetObject();
        }
    }

    template <typename TMemoryResource>
    template <typename TObject>
    MemoryAddress ScopeAllocator<TMemoryResource>::AllocateObjectWithFinalizer(Alignment alignment)
    {
        auto object_size = BytesOf<TObject>();
        
        if constexpr(std::is_trivially_destructible_v<TObject>)
        {
            return memory_resource_.Allocate(object_size, alignment).Begin();
        }
        else
        {
            auto finalizer_size = BytesOf<Finalizer>();

            auto padding_size = alignment - 1_Bytes;

            auto buffer = memory_resource_.Allocate(object_size + finalizer_size + padding_size);

            auto finalizer = ((buffer + finalizer_size).GetAligned(alignment) - finalizer_size).Begin().As<Finalizer>();

            ConstructFinalizer<TObject>(*finalizer);

            return finalizer->GetObject();
        }
    }

    template <typename TMemoryResource>
    template <typename TObject>
    void ScopeAllocator<TMemoryResource>::ConstructFinalizer(Finalizer& finalizer)
    {
        finalizer.next_ = finalizers_;
        finalizer.destructor_ = &Destructor<TObject>;

        finalizers_ = &finalizer;
    }

    template <typename TMemoryResource>
    inline void ScopeAllocator<TMemoryResource>::Finalizer::operator()()
    {
        destructor_(GetObject());
    }

    template <typename TMemoryResource>
    inline void* ScopeAllocator<TMemoryResource>::Finalizer::GetObject()
    {
        return *(MemoryAddress(this) + BytesOf<Finalizer>());
    }

    template <typename TMemoryResource>
    inline ScopeAllocator<TMemoryResource> MakeScopeAllocator(TMemoryResource& memory_resource)
    {
        return ScopeAllocator<TMemoryResource>(memory_resource);
    }

}

