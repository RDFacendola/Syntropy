#include "reflection/interfaces/property_interfaces.h"

namespace syntropy::reflection
{
    /************************************************************************/
    /* READABLE                                                             */
    /************************************************************************/

    Any Readable::operator()(const Any& instance) const
    {
        return reader_(instance);
    }

}
