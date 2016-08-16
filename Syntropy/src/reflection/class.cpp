#include "class.h"

#include <stdexcept>

#include "reflection.h"

using namespace syntropy;
using namespace syntropy::reflection;

//////////////// CLASS ////////////////

Class::Class(std::unique_ptr<IClassDefinition> definition) :
	definition_(std::move(definition)){

	if (!definition_) {

		throw std::invalid_argument("A class definition may not be null.");

	}

    class_id_ = 42; // TODO: Generate an unique prime number

    Reflection::GetInstance().Register(*this);

}

bool Class::IsBaseOf(const Class& other) const noexcept {

    return other.class_id_ % class_id_ == 0;

}

