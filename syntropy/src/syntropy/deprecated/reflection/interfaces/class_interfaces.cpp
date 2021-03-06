#include "syntropy/reflection/interfaces/class_interfaces.h"

namespace Syntropy::reflection
{
    /************************************************************************/
    /* ENUMERATION                                                          */
    /************************************************************************/

    Any Enumeration::GetValueByName(const Label& value_name) const
    {
        auto it = values_.find(value_name);

        return it != values_.end() ? it->second : Any();
    }
}