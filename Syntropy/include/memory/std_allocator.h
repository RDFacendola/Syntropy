
/// \file std_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains wrapper classes for standard allocators.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "memory/allocator.h"

/// \brief Instantiate a new object via the standard allocator wrapper.
#define SYNTROPY_STD_NEW \
    SYNTROPY_NEW(g_std_allocator)

/// \brief Delete an object created via the standard allocator wrapper.
#define SYNTROPY_STD_DELETE(ptr) \
    SYNTROPY_DELETE(g_std_allocator, ptr)

/// \brief Allocate a new buffer via the standard allocator wrapper.
#define SYNTROPY_STD_ALLOC(size) \
    SYNTROPY_ALLOC(g_std_allocator)

/// \brief Free a buffer allocated via the standard allocator wrapper.
#define SYNTROPY_STD_FREE(ptr) \
    SYNTROPY_FREE(g_std_allocator, ptr)

namespace syntropy
{

    /// \brief Simple allocator that wraps the standard new \ delete operators.
    /// Be careful mixing the usage of this allocator with others.
    /// \author Raffaele D. Facendola - February 2017
    class STDAllocator : public Allocator
    {
    public:

        /// \brief Name of the allocator.
        static const char* kName;

        /// \brief Default constructor.
        STDAllocator();

        virtual void* Allocate(size_t size) override;

        virtual void* Allocate(size_t size, size_t alignment) override;

        virtual void Free(void* block) override;

        virtual bool Belongs(void* block) const;

        virtual size_t GetMaxAllocationSize() const;

    };

    /// \brief Wrapper around a standard allocator.
    /// \author Raffaele D. Facendola - February 2017
    extern STDAllocator g_std_allocator;

}

