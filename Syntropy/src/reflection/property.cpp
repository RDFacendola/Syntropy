#include "reflection/property.h"

namespace syntropy::reflection
{

    /************************************************************************/
    /* PROPERTY                                                             */
    /************************************************************************/

    Property::Property(Property&& other)
        : MultiInterfaceMixin(std::move(other))
        , name_(std::move(other.name_))
        , type_(other.type_)
    {

    }

    const HashedString& Property::GetName() const noexcept
    {
        return name_;
    }

    const Type& Property::GetType() const noexcept
    {
        return type_;
    }
}