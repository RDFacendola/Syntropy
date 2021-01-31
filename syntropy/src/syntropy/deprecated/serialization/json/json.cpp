#include "syntropy/serialization/json/json.h"

#include "syntropy/contexts.h"

#include "syntropy/diagnostics/log.h"

namespace Syntropy::serialization
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    const reflection::Class* GetClassFromJSON(const JSON& json, const reflection::Class* base_class)
    {
        auto it = json.find(kClassToken);

        if (it == json.end())
        {
            return base_class;      // No class specified: return the base class.
        }

        if (!it->is_string())
        {
            SYNTROPY_WARNING((SerializationCtx), "Expected a string value for the property '", kClassToken, "'.");
            return nullptr;
        }

        auto concrete_class = reflection::GetClass(it->get<String>());

        if (concrete_class == nullptr)
        {
            SYNTROPY_WARNING((SerializationCtx), "Unrecognized class '", it->get<String>(), "'. Did you forget to add it to the reflection system?");
            return nullptr;
        }

        if(!concrete_class->IsA(*base_class))
        {
            SYNTROPY_WARNING((SerializationCtx), "Cannot deserialize an object of type '", base_class, "' from type ", it->get<String>(), ".");
            return nullptr;
        }

        return concrete_class;
    }
}
