#include "reflection/property.h"

namespace syntropy {

    namespace reflection {

        //////////////// PROPERTY ////////////////

        Property::Property(Property&& other) noexcept
            : name_(std::move(other.name_))
            , type_(other.type_)
            , getter_(std::move(other.getter_))
            , setter_(std::move(other.setter_))
            , interfaces_(std::move(other.interfaces_)){

        }

        const HashedString& Property::GetName() const noexcept {

            return name_;

        }

        const Type& Property::GetType() const noexcept {

            return type_;

        }
        
    }

}