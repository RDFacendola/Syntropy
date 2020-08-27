
/// \file scope_allocator.h
/// \brief This header is part of the Syntropy memory module. It contains scope-based allocators used to automatically destruct objects on rewindable allocators.
///
/// \author Raffaele D. Facendola - March 2017

#pragma once

#include "syntropy/language/type_traits.h"

#include "syntropy/language/fundamentals.h"

#include "syntropy/memory/bytes.h"
#include "syntropy/memory/alignment.h"
#include "syntropy/memory/byte_span.h"

#include "syntropy/diagnostics/assert.h"

namespace Syntropy
{
    /************************************************************************/
    /* SCOPE ALLOCATOR <ALLOCATOR>                                          */
    /************************************************************************/

    /// \brief Utility allocator that sits on top of another rewindable allocator and uses RAII paradigm to automatically destruct objects when going out of scope.
    /// \tparam TAllocator Type of the underlying allocator. The allocator must be rewindable by means of Checkpoint()\Rewind() method pairs.
    /// \author Raffaele D. Facendola - January 2017
    /// \remarks Based on https://media.contentapi.ea.com/content/dam/eacom/frostbite/files/scopestacks-public.pdf
    template <typename TAllocator>
    class ScopeAllocator
    {

        /// \brief Type of the allocator checkpoint.
        using TCheckpoint = decltype(Declval<TAllocator>().Checkpoint());

    public:

        /// \brief Create a new scope allocator.
        /// \param allocator Underlying allocator.
        ScopeAllocator(TAllocator& allocator) noexcept;

        /// \brief Default destructor.
        /// Destroy all the objects that were allocated from this allocator and restore the underlying allocator status.
        ~ScopeAllocator() noexcept;

        /// \brief Create a new object of type TObject.
        template <typename TObject, typename... TArguments>
        RWPointer<TObject> New(TArguments&&... arguments) noexcept;

        /// \brief Create a new aligned object of type TObject.
        template <typename TObject, typename... TArguments>
        RWPointer<TObject> AlignedNew(Alignment alignment, TArguments&&... arguments) noexcept;

    private:

        struct Finalizer;

        /// \brief Destroy an object of type TObject.
        template <typename TObject>
        static void Destructor(RWTypelessPtr object) noexcept;

        /// \brief Allocate an object of type TObject along with its finalizer (if required).
        template <typename TObject>
        RWByteSpan AllocateObjectWithFinalizer() noexcept;

        /// \brief Allocate an aligned object of type TObject along with its finalizer (if required).
        template <typename TObject>
        RWByteSpan AllocateObjectWithFinalizer(Alignment alignment) noexcept;

        /// \brief Construct the finalizer for the type TObject.
        template <typename TObject>
        void ConstructFinalizer(Finalizer& finalizer) noexcept;

        /// \brief Underlying allocator.
        TAllocator& allocator_;

        /// \brief First finalizer.
        RWPointer<Finalizer> finalizers_{ nullptr };

        /// \brief Allocator checkpoint prior to scope allocator ctor.
        TCheckpoint checkpoint_;
    };

    /************************************************************************/
    /* SCOPE ALLOCATOR <ALLOCATOR> :: FINALIZER                             */
    /************************************************************************/

    /// \brief Finalizer object used to destroy non-trivially-destructible objects.
    template <typename TAllocator>
    struct ScopeAllocator<TAllocator>::Finalizer
    {
        /// \brief Type of a finalizer.
        using TFinalizer = void(*)(RWTypelessPtr);

        /// \brief Destroy the object after this finalizer.
        void operator()() noexcept;

        /// \brief Get the object bound to this finalizer.
        RWTypelessPtr GetObject() noexcept;

        /// \brief Concrete object destructor.
        TFinalizer destructor_;

        /// \brief Next finalizer.
        RWPointer<Finalizer> next_{ nullptr };
    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    /// \brief Create a new scope allocator by deducing template from arguments.
    template <typename TAllocator>
    ScopeAllocator<TAllocator> MakeScopeAllocator(TAllocator& allocator) noexcept;

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // ScopeAllocator<TAllocator>.
    // ===========================

    template <typename TAllocator>
    inline ScopeAllocator<TAllocator>::ScopeAllocator(TAllocator& allocator) noexcept
        : allocator_(allocator)
        , finalizers_(nullptr)
        , checkpoint_(allocator.Checkpoint())
    {

    }

    template <typename TAllocator>
    inline ScopeAllocator<TAllocator>::~ScopeAllocator() noexcept
    {
        for(;finalizers_; finalizers_ = finalizers_->next_)
        {
            (*finalizers_)();
        }

        allocator_.Rewind(checkpoint_);
    }

    template <typename TAllocator>
    template <typename TObject, typename... TArguments>
    inline TObject* ScopeAllocator<TAllocator>::New(TArguments&&... arguments) noexcept
    {
        auto storage = AllocateObjectWithFinalizer<TObject>();

        return new (*storage) TObject(Forward<TArguments>(arguments)...);
    }

    template <typename TAllocator>
    template <typename TObject, typename... TArguments>
    inline  TObject* ScopeAllocator<TAllocator>::AlignedNew(Alignment alignment, TArguments&&... arguments) noexcept
    {
        auto storage = AllocateObjectWithFinalizer<TObject>(alignment);

        return new (*storage) TObject(Forward<TArguments>(arguments)...);
    }

    template <typename TAllocator>
    template <typename TObject>
    inline void ScopeAllocator<TAllocator>::Destructor(RWTypelessPtr instance) noexcept
    {
        auto& type_from = typeid(object);
        auto& type_to = typeid(TObject);

        SYNTROPY_UNDEFINED_BEHAVIOR(type_from == type_to, "Dynamic type mismatch.");

        ToPointer<TObject>(instance)->~TObject();
    }

    template <typename TAllocator>
    template <typename TObject>
    RWByteSpan ScopeAllocator<TAllocator>::AllocateObjectWithFinalizer() noexcept
    {
        auto size = Memory::SizeOf<TObject>();
        auto alignment = Memory::AlignmentOf<TObject>();

        if constexpr(std::is_trivially_destructible_v<TObject>)
        {
            return allocator_.Allocate(size, alignment);
        }
        else
        {
            auto finalizer_size = Memory::SizeOf<Finalizer>();

            auto& finalizer = ToRWSpan<Finalizer>(allocator_.Allocate(size + finalizer_size, size))[0];

            ConstructFinalizer<TObject>(finalizer);

            return finalizer.GetObject();
        }
    }

    template <typename TAllocator>
    template <typename TObject>
    RWByteSpan ScopeAllocator<TAllocator>::AllocateObjectWithFinalizer(Alignment alignment) noexcept
    {
        auto size = Memory::SizeOf<TObject>();
        
        if constexpr(std::is_trivially_destructible_v<TObject>)
        {
            return allocator_.Allocate(size, alignment);
        }
        else
        {
            auto finalizer_size = Memory::SizeOf<Finalizer>();

            auto padding_size = alignment - 1_Bytes;

            auto buffer = allocator_.Allocate(size + finalizer_size + padding_size);

            auto finalizer = ((buffer + finalizer_size).GetAligned(alignment) - finalizer_size).Begin().As<Finalizer>();

            ConstructFinalizer<TObject>(*finalizer);

            return finalizer->GetObject();
        }
    }

    template <typename TAllocator>
    template <typename TObject>
    void ScopeAllocator<TAllocator>::ConstructFinalizer(Finalizer& finalizer) noexcept
    {
        finalizer.next_ = finalizers_;
        finalizer.destructor_ = &Destructor<TObject>;

        finalizers_ = &finalizer;
    }

    //ScopeAllocator<TAllocator>::Finalizer.

    template <typename TAllocator>
    inline void ScopeAllocator<TAllocator>::Finalizer::operator()() noexcept
    {
        destructor_(GetObject());
    }

    template <typename TAllocator>
    inline RWTypelessPtr ScopeAllocator<TAllocator>::Finalizer::GetObject() noexcept
    {
        return ToRWBytePtr(this) + Memory::SizeOf<Finalizer>();
    }

    // Non-member functions.

    template <typename TAllocator>
    inline ScopeAllocator<TAllocator> MakeScopeAllocator(TAllocator& allocator) noexcept
    {
        return ScopeAllocator<TAllocator>(allocator);
    }

}

