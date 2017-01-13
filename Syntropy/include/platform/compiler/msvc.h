
/// \file msvc.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains the definition of MSVC-specific functionalities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#ifdef _MSC_VER

#include <intrin.h>

#include "platform/builtin.h"

/// \brief Causes the debugger to break.
#define SYNTROPY_TRAP \
    __debugbreak()

/// \brief Expands to the current function name.
#define SYNTROPY_FUNCTION \
    __FUNCTION__

namespace syntropy
{
    namespace platform
    {

        /// \brief Exposes MSVC-specific built-in functionalities.
        /// \author Raffaele D. Facendola - December 2016
        class MSVCBuiltIn : public BuiltIn
        {
        public:


            /// \brief Get the singleton instance.
            /// \return Returns the singleton instance.
            static platform::BuiltIn& GetInstance();

            virtual uint64_t GetMostSignificantBit(uint64_t number) const override;

        private:

            /// \brief Default constructor.
            MSVCBuiltIn() = default;

        };

        ///< \brief Utility type definition so that the code can refer to "platform::PlatformBuiltIn" without having to know the actual concrete class.
        using PlatformBuiltIn = MSVCBuiltIn;

    }
}

#endif
