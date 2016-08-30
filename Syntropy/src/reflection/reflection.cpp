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

bool Reflection::Register(Class& class_instance) {

    bool result = true;

    std::hash<HashedString>::result_type name_hash;

    // Register each alias as a different entry

    for (auto&& name_alias : class_instance.GetNames()) {

        name_hash = std::hash<HashedString>()(name_alias);

        if (classes_.find(name_hash) == classes_.end()) {

            classes_.emplace(std::make_pair(name_hash, &class_instance));

        }
        else {

            result = false;

        }

    }

    return result;

}
