#include "synchrony/network.h"

#include "synchrony/platform/windows_network.h"

namespace synchrony
{
    /************************************************************************/
    /* NETWORKING                                                           */
    /************************************************************************/

    bool Network::Startup()
    {
        return PlatformNetwork::Startup();
    }

    bool Network::Shutdown()
    {
        return PlatformNetwork::Shutdown();
    }

}
