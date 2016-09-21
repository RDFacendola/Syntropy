#include "reflection/property.h"

namespace syntropy {

    namespace reflection {

        //////////////// PROPERTY ////////////////

        Property::Property(const char* name)
            : name_(name) {}

        const HashedString& Property::GetName() const noexcept {

            return name_;

        }

        //////////////// INTERFACE DECLARATION ////////////////
        
        InterfaceDeclaration::InterfaceDeclaration(Property& property)
            : property_(property){}

    }

}