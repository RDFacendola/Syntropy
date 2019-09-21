
/// \file ip.h
/// \brief This header is part of the synchrony network system. It contains base definitions for the Internet Protocol.
///
/// \author Raffaele D. Facendola - 2019

#pragma once

#include <cstdint>

namespace synchrony
{
    /************************************************************************/
    /* IP VERSION                                                           */
    /************************************************************************/

    /// \brief Internet protocol version.
    /// \author Raffaele D. Facendola - 2019.
    enum class IPVersion : std::uint8_t
    {
        /// \brief IP version 4.
        kIPv4 = 0,

        /// \brief IP version 6.
        kIPv6 = 1,
    };

}

