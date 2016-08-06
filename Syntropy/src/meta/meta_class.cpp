#include "meta_class.h"

#include <vector>
#include <numeric>

#include <syntropy.h>

namespace syntropy {
    
    //////////////// META REGISTRY ////////////////

    MetaClassRegistry::MetaClassRegistry() {

        meta_classes_.reserve(2048);

    }

    const MetaClass* MetaClassRegistry::GetClass(const HashedString& class_name) {

        auto it = meta_classes_.find(class_name);

        return (it != meta_classes_.end()) 
               ? it->second 
               : nullptr;

    }

    void MetaClassRegistry::RegisterMetaClass(MetaClass& meta_class) {

        assert(meta_classes_.find(meta_class.GetName()) == meta_classes_.end());

        meta_classes_.insert(std::make_pair(meta_class.GetName(), std::addressof(meta_class)));

    }
 
    //////////////// META CLASS ////////////////

    MetaClass::MetaClass(std::unique_ptr<MetaClassDeclaration> declaration) :
        class_(std::move(declaration)) {

        class_id_ = 42;

        MetaClassRegistry::GetInstance().RegisterMetaClass(*this);
        
    }

}