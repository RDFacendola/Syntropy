#include "class.h"

#include "reflection.h"

using namespace syntropy;
using namespace syntropy::reflection;

//////////////// CLASS ////////////////

void Class::Initialize() {

    class_id_ = 42; // TODO: Generate an unique prime number

    Reflection::GetInstance().Register(*this);

}

bool Class::IsBaseOf(const Class& other) const noexcept {

    return other.class_id_ % class_id_ == 0;

}

