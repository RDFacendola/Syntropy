#include "reflection/class.h"

#include "reflection/reflection.h"

namespace syntropy {

    namespace reflection {

        //////////////// CLASS ////////////////

        bool Class::operator ==(const Class& other) const noexcept {

            // The two classes are the same if they share the same address (since we are dealing with singleton)
            // or any ancestor does.
    
            return (this == &other) ||
                   std::any_of(GetBaseClasses().begin(), GetBaseClasses().end(), [&other](const Class* base_class) { return *base_class == other; });
    
        }

        bool Class::operator !=(const Class& other) const noexcept {

            return (this != &other) &&
                   std::all_of(GetBaseClasses().begin(), GetBaseClasses().end(), [&other](const Class* base_class) { return *base_class != other; });

        }

        //////////////// STREAM INSERTION ////////////////

        std::ostream& operator<<(std::ostream& out, const Class& class_instance) {

            out << class_instance.GetName();

            return out;

        }

    }

}


