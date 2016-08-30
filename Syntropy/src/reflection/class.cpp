#include "class.h"

#include "reflection.h"

using namespace syntropy;
using namespace syntropy::reflection;

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