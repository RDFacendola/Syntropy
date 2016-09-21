#include "reflection/property.h"

namespace syntropy {

    namespace reflection {

        //////////////// PROPERTY ////////////////

        Property::Property(Property&& other) noexcept
            : name_(std::move(other.name_))
            , property_(std::move(other.property_))
            , interfaces_(std::move(other.interfaces_)){}

        const HashedString& Property::GetName() const noexcept {

            return name_;

        }

        const Type& Property::GetType() const noexcept {

            return property_->GetType();

        }

        void Property::AddInterfaces() {}

        //////////////// INTERFACE DECLARATION ////////////////
        
        InterfaceDeclaration::InterfaceDeclaration(Property& property)
            : property_(property){}

    }

}