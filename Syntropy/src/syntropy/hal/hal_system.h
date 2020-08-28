
/// \file hal_system.h
/// \brief This header is part of the Syntropy hardware abstraction layer module. It contains methods to query system capabilities.
///
/// \author Raffaele D. Facendola - 2020

#pragma once

#include "syntropy/language/foundation.h"
#include "syntropy/platform/system.h"

namespace Syntropy
{
    /************************************************************************/
    /* HAL SYSTEM                                                           */
    /************************************************************************/

    /// \brief Exposes methods to query system's capabilities.
    /// \author Raffaele D. Facendola
    namespace HALSystem
    {
        /// \brief Get the current CPU infos.
        CPUInfo GetCPUInfo();

        /// \brief Get the current storage infos.
        StorageInfo GetStorageInfo();

        /// \brief Get the current memory infos.
        MemoryInfo GetMemoryInfo();

        /// \brief Get the current desktop infos.
        DisplayInfo GetDisplayInfo();

        /// \brief Get the current platform infos.
        PlatformInfo GetPlatformInfo();
    };

}