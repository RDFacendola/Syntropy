#include "class.h"

#include "reflection.h"

using namespace syntropy;
using namespace syntropy::reflection;

//////////////// CLASS ////////////////

size_t kPrimes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71 };

Class::Class(std::unique_ptr<IClassDefinition> definition)
    : definition_(std::move(definition)){

    if (!definition_) {

        throw std::invalid_argument("A class definition may not be null.");

    }

    static size_t instanceCount = 0;

    // TODO: Generate an unique prime number

    class_id_ = kPrimes[instanceCount++]; 

    for (auto&& base : definition_->GetBaseClasses()) {

        class_id_ *= base->class_id_;

    }
    
    Reflection::GetInstance().Register(*this);

}

bool Class::operator ==(const Class& other) const noexcept {

    return class_id_ % other.class_id_ == 0;

}

bool Class::operator !=(const Class& other) const noexcept {

    return class_id_ % other.class_id_ != 0;

}

//////////////// TYPE ////////////////

//void Type::BuildName(std::ostringstream& name_stream) const noexcept {
//
//    auto next = GetNext();
//
//    if (next != nullptr) {
//
//        next->BuildName(name_stream);                       // Recursion till the innermost level
//
//    }
//    else {
//
//        name_stream << GetClass().GetName().GetString();    // Append class name
//
//    }
//
//    size_t extent;
//
//    for (size_t dimension = 0; dimension < GetArrayRank(); ++dimension) {
//        
//        extent = GetArraySize(dimension);
//
//        if (extent > 0) {
//
//            name_stream << "[" << extent << "]";
//
//        }
//        else {
//
//            name_stream << "[]";
//
//        }
//                
//    }
//    
//    if (IsLValueReference()) {
//
//        name_stream << "&";
//
//    }
//
//    if (IsRValueReference()) {
//
//        name_stream << "&&";
//
//    }
//        
//    if (IsPointer()) {
//
//        name_stream << "*";
//
//    }
//
//    if (!IsArray()) {
//
//        // Caveat - arrays have the same qualifiers as their nested types: to prevent the same qualifier being printed twice, we skip this level in case of an array.
//
//        if (IsConst()) {
//            
//            name_stream << " const";
//        
//        }
//
//        if (IsVolatile()) {
//
//            name_stream << " volatile";
//
//        }
//
//    }
//    
//}
