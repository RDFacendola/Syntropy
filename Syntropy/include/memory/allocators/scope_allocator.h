
/// \file stack_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains scope-based allocators.
///
/// \author Raffaele D. Facendola - March 2017

#pragma once

#include "memory/bytes.h"
#include "memory/alignment.h"
#include "memory/memory_address.h"

#include "diagnostics/assert.h"

namespace syntropy
{
    /// \brief Utility allocator that sits on top of a rewindable allocator and handles concrete object construction and destruction via RAII.
    /// \tparam TAllocator Type of the underlying allocator. TAllocator must be a restorable allocator and expose the following methods:
    ///     SaveState() -> State
    ///     RestoreState(State) -> ?
    ///
    /// \usage ScopeAllocator<> s(restorable_allocator);
    ///        auto foo = s.New<Foo>(arg0, arg1);
    ///        (foo gets destroyed when s goes out of scope)
    ///
    /// \author Raffaele D. Facendola - January 2017
    /// \see Based on https://media.contentapi.ea.com/content/dam/eacom/frostbite/files/scopestacks-public.pdf
    template <typename TAllocator>
    class ScopeAllocator
    {
    public:

        /// \brief Create a new scope allocator.
        /// \param allocator Underlying linear allocator.
        ScopeAllocator(TAllocator& allocator);

        /// \brief Default destructor.
        /// Destroys all the objects that were allocated from this allocator and rewinds the allocator status.
        ~ScopeAllocator();

        /// \brief Allocate and construct a new object.
        /// \tparam TObject Type of the object to create.
        /// \param arguments Arguments to pass to TObject's constructor.
        /// \return Returns a pointer to a new instance of TObject.
        template <typename TObject, typename... TArguments>
        TObject* New(TArguments&&... arguments);

        /// \brief Allocate and construct a new aligned object.
        /// \tparam TObject Type of the object to create.
        /// \param alignment Alignment of the object, in bytes.
        /// \param arguments Arguments to pass to TObject's constructor.
        /// \return Returns a pointer to a new instance of TObject.
        template <typename TObject, typename... TArguments>
        TObject* AlignedNew(Alignment alignment, TArguments&&... arguments);

    private:

        /// \brief Finalizer object used to destroy non-trivially-destructible objects.
        /// Finalizers are allocated just before the object they are supposed to destroy.
        struct Finalizer
        {
            using TFinalizer = void(*)(void*);

            /// \brief Destroy the object after this finalizer.
            void operator()() const;

            /// \brief Get the object bound to this finalizer.
            void* GetObject() const;

            TFinalizer destructor_;                     ///< \brief Destructor used to destroy the object after this finalizer.

            Finalizer* next_ = nullptr;                 ///< \brief Next finalizer.
        };

        /// \brief Destroy an object of type TObject.
        /// \param instance Pointer to the instance to destroy. Must be of type TObject.
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

        TAllocator& allocator_;                         ///< \brief Underlying allocator.

        Finalizer* finalizers_ = nullptr;               ///< \brief First finalizer functor.

        MemoryAddress scope_;                           ///< \brief Allocator status to restore upon destruction.
    };

    /// \brief Utility method used to make a new scope allocator.
    /// \param allocator Underlying allocator the scope will operate on.
    /// \return Returns a new scope allocator.
    template <typename TAllocator>
    ScopeAllocator<TAllocator> MakeScopeAllocator(TAllocator& allocator);

    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    template <typename TAllocator>
    inline ScopeAllocator<TAllocator>::ScopeAllocator(TAllocator& allocator)
        : allocator_(allocator)
        , finalizers_(nullptr)
    {
        scope_ = allocator.SaveState();
    }

    template <typename TAllocator>
    inline ScopeAllocator<TAllocator>::~ScopeAllocator()
    {
        // Destroy each non-trivially-destructible object in reverse order.

        while (finalizers_)
        {
            (*finalizers_)();
            finalizers_ = finalizers_->next_;
        }

        allocator.RestoreState(scope_);                 // Restore the old state of the allocator.
    }

    template <typename TAllocator>
    template <typename TObject, typename... TArguments>
    inline TObject* ScopeAllocator<TAllocator>::New(TArguments&&... arguments)
    {
        auto storage = AllocateObjectWithFinalizer<TObject>();

        return new (*storage) TObject(std::forward<TArguments>(arguments)...);          // Construct the object.
    }

    template <typename TAllocator>
    template <typename TObject, typename... TArguments>
    inline  TObject* ScopeAllocator<TAllocator>::AlignedNew(Alignment alignment, TArguments&&... arguments)
    {
        auto storage = AllocateObjectWithFinalizer<TObject>(alignment);

        SYNTROPY_ASSERT(storage.IsAlignedTo(alignment));

        return new (*storage) TObject(std::forward<TArguments>(arguments)...);          // Construct the object.
    }

    template <typename TAllocator>
    template <typename TObject>
    inline void ScopeAllocator<TAllocator>::Destructor(void* instance)
    {
        static_cast<TObject*>(instance)->~TObject();
    }

    template <typename TAllocator>
    template <typename TObject>
    MemoryAddress ScopeAllocator<TAllocator>::AllocateObjectWithFinalizer()
    {
        auto object_size = Bytes(sizeof(TObject));

        if constexpr(std::is_trivially_destructible_v<TObject>)
        {
            return allocator_.Allocate(object_size);
        }
        else
        {
            auto finalizer_size = Bytes(sizeof(Finalizer));

            auto finalizer = allocator_.Allocate(object_size + finalizer_size).As<Finalizer>();

            ConstructFinalizer<TObject>(*finalizer);

            return finalizer.GetObject();
        }
    }

    template <typename TAllocator>
    template <typename TObject>
    MemoryAddress ScopeAllocator<TAllocator>::AllocateObjectWithFinalizer(Alignment alignment)
    {
        auto object_size = Bytes(sizeof(TObject));
        
        if constexpr(std::is_trivially_destructible_v<TObject>)
        {
            return allocator_.Allocate(object_size, alignment);
        }
        else
        {
            auto finalizer_size = Bytes(sizeof(Finalizer));
            auto padding_size = alignment - 1_Bytes;

            auto buffer = allocator_.Allocate(object_size + finalizer_size + padding_size);

            auto finalizer = ((buffer + finalizer_size).GetAligned(alignment) - finalizer_size).As<Finalizer>();

            ConstructFinalizer<TObject>(*finalizer);

            return finalizer.GetObject();
        }
    }

    template <typename TAllocator>
    template <typename TObject>
    void ScopeAllocator<TAllocator>::ConstructFinalizer(Finalizer& finalizer)
    {
        finalizer.next_ = finalizers_;
        finalizer.destructor_ = &Destructor<TObject>;

        finalizers_ = &finalizer;
    }

    template <typename TAllocator>
    inline void ScopeAllocator<TAllocator>::Finalizer::operator()() const
    {
        destructor_(GetObject());
    }

    template <typename TAllocator>
    inline void* ScopeAllocator<TAllocator>::Finalizer::GetObject() const
    {
        return MemoryAddress(this) + Bytes(sizeof(Finalizer));
    }

    template <typename TAllocator>
    ScopeAllocator<TAllocator> MakeScopeAllocator(TAllocator& allocator)
    {
        return ScopeAllocator<TAllocator>(allocator);
    }

}

