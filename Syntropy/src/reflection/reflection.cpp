#include "reflection.h"

using namespace syntropy;
using namespace syntropy::reflection;

//////////////// REFLECTION ////////////////

Reflection::Reflection() {

    classes_.reserve(2048);

}

const Class* Reflection::GetClass(const HashedString& class_name) noexcept {

    auto it = classes_.find(std::hash<HashedString>()(class_name));

    return (it != classes_.end())
           ? it->second
           : nullptr;

}

void Reflection::Register(Class& class_instance) {

    assert(classes_.find(std::hash<HashedString>()(class_instance.GetName())) == classes_.end());

    classes_.emplace(std::make_pair(std::hash<HashedString>()(class_instance.GetName()),
                                    std::addressof(class_instance)));

}
