
/// \file network.h
/// \brief This header is part of the synchrony network system. It contains basic functionalities used to startup and shutdown the networking service.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#ifdef _WIN64

namespace synchrony
{
    /************************************************************************/
    /* WINDOWS NETWORK                                                      */
    /************************************************************************/

    /// \brief Contains network-related functions under Windows OS.
    /// \author Raffaele D. Facendola - September 2019.
    namespace WindowsNetwork
    {
        /// \brief Startup the networking service.
        /// \return Returns true if the networking service could be initialized successfully, returns false otherwise.
        bool Startup();

        /// \brief Shutdown the networking service.
        /// If the networking service wasn't running this method does nothing.
        /// \return Returns true if the networking service could be shutdown successfully, returns false otherwise.
        bool Shutdown();
    }

    namespace PlatformNetwork = WindowsNetwork;
}

#endif
