#include "reflection/property.h"

namespace syntropy {

    namespace reflection {

        /************************************************************************/
        /* PROPERTY                                                             */
        /************************************************************************/

        Property::Property(Property&& other)
            : name_(std::move(other.name_))
            , type_(other.type_)
            , interfaces_(std::move(other.interfaces_))
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

}