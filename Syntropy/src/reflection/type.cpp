#include "type.h"

#include "class.h"

namespace syntropy {

    namespace reflection {

        bool Type::operator ==(const Type& other) const noexcept {

            return GetTypeInfo() == other.GetTypeInfo() &&
                   GetClass() == other.GetClass();

        }

    }

}

