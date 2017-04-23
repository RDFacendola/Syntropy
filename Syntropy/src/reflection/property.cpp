#include "reflection/property.h"

namespace syntropy {

    namespace reflection {

        /************************************************************************/
        /* PROPERTY                                                             */
        /************************************************************************/

        Property::Property(Property&& other)
            : name_(std::move(other.name_))
            , interfaces_(std::move(other.interfaces_))
            , property_(std::move(other.property_))
        {

        }

        const HashedString& Property::GetName() const noexcept
        {
            return name_;
        }

        const Type& Property::GetType() const noexcept
        {
            return property_->GetType();
        }

    }

}