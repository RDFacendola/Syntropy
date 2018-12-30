
/// \file system.h
/// \brief This header is part of the syntropy HAL (hardware abstraction layer) system. It contains classes used to query system's capabilities.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <string>
#include <vector>

namespace syntropy::platform
{

    /// \brief CPU architectures.
    /// \author Raffaele D. Facendola
    enum class CPUArchitecture
    {
        kx86,                       ///< \brief x86 architecture.
        kx64,                       ///< \brief x64 architecture.
        kARM,                       ///< \brief ARM architecture.
        kUnknown,                   ///< \brief Unknown architecture.
    };

    /// \brief Describes the CPU's capabilities.
    /// \author Raffaele D. Facendola
    struct CPUInfo
    {
        uint64_t frequency_;                ///< \brief Frequency of each core in Hz.
        uint32_t cores_;                    ///< \brief Number of logical cores.
        CPUArchitecture architecture_;      ///< \brief CPU architecture.
    };

    /// \brief Describes a particular drive.
    /// \author Raffaele D. Facendola
    struct DriveInfo 
    {
        std::string label_;                 ///< \brief Unit letter or label.
        uint64_t total_space_;              ///< \brief Total drive space, in bytes.
        uint64_t available_space_;          ///< \brief Total available drive space, in bytes.
    };

    /// \brief Describes the storage capabilities.
    /// \author Raffaele D. Facendola
    struct StorageInfo
    {
        std::vector<DriveInfo> drives_;     ///< \brief List of fixed, physical drives currently available.
    };

    /// \brief Describes the system memory.
    /// \author Raffaele D. Facendola
    struct MemoryInfo
    {
        uint64_t total_physical_memory_;        ///< \brief Total physical memory, in bytes.
        uint64_t total_virtual_memory_;         ///< \brief Total virtual address space for the current process, in bytes.
        uint64_t total_page_memory_;            ///< \brief Total page memory, in bytes.
        uint64_t available_physical_memory_;    ///< \brief Available physical memory, in bytes.
        uint64_t available_virtual_memory_;	    ///< \brief Available virtual address space for the current process, in bytes.
        uint64_t available_page_memory_;        ///< \brief Available page memory, in bytes.
    };

    /// \brief Describes a single monitor.
    /// \author Raffaele D. Facendola - December 2016
    struct MonitorInfo
    {
        std::string monitor_name_;              ///< \brief Name of the monitor.
        std::string adapter_name_;              ///< \brief Name of the adapter (GPU) the monitor is attached to.
        uint32_t width_;                        ///< \brief Horizontal resolution.
        uint32_t height_;                       ///< \brief Vertical resolution.
        float refresh_rate_;                    ///< \brief Refresh rate, in Hz.
        bool is_primary_;                       ///< \brief Whether this monitor is the primary one.
    };

    /// \brief Describes the desktop.
    /// \author Raffaele D. Facendola
    struct DisplayInfo
    {
        std::vector<MonitorInfo> monitors_;     ///< \brief List of available monitors.
    };

    /// \brief Operating system.
    /// \author Raffaele D. Facendola
    enum class OperatingSystem 
    {
        kWindows                                ///< Windows OS
    };

    /// \brief Describes the platform capabilities.
    /// \author Raffaele D. Facendola - December 2016
    struct PlatformInfo
    {
        OperatingSystem operating_system_;      ///< \brief Current operating system.
    };

    /************************************************************************/
    /* SYSTEM                                                               */
    /************************************************************************/

    /// \brief Exposes methods to query system's capabilities.
    /// \author Raffaele D. Facendola
    class System
    {
    public:

        /// \brief Get the current CPU infos.
        /// \return Returns the current CPU infos.
        static CPUInfo GetCPUInfo();

        /// \brief Get the current storage infos.
        /// \return Returns the current storage infos.
        static StorageInfo GetStorageInfo();

        /// \brief Get the current memory infos.
        /// \return Returns the current memory infos.
        static MemoryInfo GetMemoryInfo();

        /// \brief Get the current desktop infos.
        /// \return Returns the current desktop infos.
        static DisplayInfo GetDisplayInfo();

        /// \brief Get the current platform infos.
        /// \return Returns the current platform infos.
        static PlatformInfo GetPlatformInfo();
    };
}