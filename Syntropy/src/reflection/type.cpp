#include "type.h"

#include "class.h"

namespace syntropy {

    namespace reflection {

        bool Type::operator ==(const Type& other) const noexcept {

            return GetCommonTypeInfo() == other.GetCommonTypeInfo() &&
                   GetClass() == other.GetClass();

        }

        bool Type::operator!=(const Type& other) const noexcept {

            return GetCommonTypeInfo() != other.GetCommonTypeInfo() ||
                   GetClass() != other.GetClass();

        }

    }

}

