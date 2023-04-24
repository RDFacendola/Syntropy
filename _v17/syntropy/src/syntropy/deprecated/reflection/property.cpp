#include "syntropy/reflection/property.h"

namespace Syntropy::reflection
{

    /************************************************************************/
    /* PROPERTY                                                             */
    /************************************************************************/

    Property::Property(Property&& other)
        : MultiInterfaceMixin(Move(other))
        , name_(Move(other.name_))
        , type_(other.type_)
    {

    }

    const Label& Property::GetName() const noexcept
    {
        return name_;
    }

    const Type& Property::GetType() const noexcept
    {
        return type_;
    }
}