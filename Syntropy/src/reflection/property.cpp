#include "property.h"

namespace syntropy {

    namespace reflection {

        //////////////// PROPERTY ////////////////

        const HashedString& Property::GetName() const noexcept {

            return name_;

        }

        const Type& Property::GetType() const noexcept {

            return type_;

        }

    }

}