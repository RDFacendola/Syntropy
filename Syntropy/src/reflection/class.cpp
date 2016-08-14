#include "class.h"

using namespace syntropy;
using namespace syntropy::reflection;

//////////////// CLASS ////////////////

Class::Class(std::unique_ptr<IClassDeclaration> declaration) :
    declaration_(std::move(declaration)) {

    class_id_ = 42;

    ClassRegistry::GetInstance().Register(*this);

}

bool Class::IsDerived(const Class& other) const noexcept {

    return class_id_ % other.class_id_ == 0;

}

//////////////// CLASS REGISTRY ////////////////

ClassRegistry::ClassRegistry() {

    classes_.reserve(2048);

}

const Class* ClassRegistry::GetClass(const HashedString& class_name) noexcept {

    auto it = classes_.find(std::hash<HashedString>()(class_name));

    return (it != classes_.end())
            ? it->second
            : nullptr;

}

void ClassRegistry::Register(Class& class_instance) {

    assert(classes_.find(std::hash<HashedString>()(class_instance.GetName())) == classes_.end());

    classes_.emplace(std::make_pair(std::hash<HashedString>()(class_instance.GetName()),
                                    std::addressof(class_instance)));

}
