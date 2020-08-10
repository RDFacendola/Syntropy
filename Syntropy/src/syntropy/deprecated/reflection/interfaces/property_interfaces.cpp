#include "syntropy/reflection/interfaces/property_interfaces.h"

namespace Syntropy::reflection
{
    /************************************************************************/
    /* READABLE                                                             */
    /************************************************************************/

    Any Readable::operator()(const Any& instance) const
    {
        return reader_(instance);
    }

}
