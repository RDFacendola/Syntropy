
/// \file std_allocator.h
/// \brief This header is part of the syntropy memory management system. It contains wrapper classes for standard allocators.
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include "memory.h"

namespace syntropy
{

    /// \brief Allocator that wraps the standard new \ delete operators.
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

    };

    /// \brief Wrapper around a standard allocator.
    /// \author Raffaele D. Facendola - February 2017
    extern STDAllocator g_std_allocator;

}

