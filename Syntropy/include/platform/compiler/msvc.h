/// \file msvc.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains the definition of MSVC-specific functionalities.
///
/// Do not include this header directly. Use compiler.h instead.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#ifndef SYNTROPY_COMPILER_INCLUDE_GUARD
#error "You may not include this header directly. Use compiler.h instead."
#endif

#ifdef _MSC_VER

#include "platform/builtin.h"

namespace syntropy
{
    namespace platform
    {

        /// \brief Exposes MSVC-specific built-in functionalities.
        /// \author Raffaele D. Facendola - December 2016
        class PlatformBuiltIn
        {
        public:

            /// \brief Get the index of the most significant bit set.
            /// \return Returns the index of the most significant bit set. Undefined behavior if number is 0.
            static uint64_t GetMostSignificantBit(uint64_t number);

            /// \brief Get the index of the least significant bit set.
            /// \return Returns the index of the least significant bit set. Undefined behavior if number is 0.
            static uint64_t GetLeastSignificantBit(uint64_t number);

        };

    }
}

#endif
