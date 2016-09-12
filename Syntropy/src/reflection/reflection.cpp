#include "reflection/reflection.h"

using namespace syntropy;
using namespace syntropy::reflection;

//////////////// REFLECTION ////////////////

Reflection::Reflection() {

    classes_.reserve(2048);

}

const Class* Reflection::GetClass(const HashedString& class_name) const noexcept {

    auto it = classes_.find(std::hash<HashedString>()(class_name));

    return (it != classes_.end())
           ? it->second
           : nullptr;

}

void Reflection::Register(Class& class_instance) {

    std::hash<HashedString>::result_type name_hash;

    auto it = classes_.begin();

    // Register each alias as a different entry

    for (auto&& name_alias : class_instance.GetNames()) {

        name_hash = std::hash<HashedString>()(name_alias);

        it = classes_.find(name_hash);

        if (it == classes_.end()) {

            classes_.emplace(std::make_pair(name_hash, &class_instance));

        }
        else {

            // In order to prevent declaration order-dependent behaviors when a name clash occurs, the existing registration is invalidated and the new one is ignored.

            it->second = nullptr;

        }

    }

}
