#include "reflection/property.h"

namespace syntropy {

    namespace reflection {

        /************************************************************************/
        /* PROPERTY                                                             */
        /************************************************************************/

        Property::Property(std::unique_ptr<BaseProperty> property)
            : property_(std::move(property))
        {

        }

        Property::Property(Property&& other)
            : property_(std::move(other.property_))
        {

        }

        const HashedString& Property::GetName() const noexcept
        {
            return property_->GetName();
        }

        const Type& Property::GetType() const noexcept
        {
            return property_->GetType();
        }

        /************************************************************************/
        /* BASE PROPERTY                                                        */
        /************************************************************************/

        BaseProperty::BaseProperty(const HashedString& name)
            : name_(name)
        {

        }

        const HashedString& BaseProperty::GetName() const noexcept
        {
            return name_;
        }

    }

}