
/// \file fundamental_types.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <memory>
#include <cstddef>

#include "class.h"

#define SYN_DEFINE_FUNDAMENTAL_TYPE(type) \
template <> \
struct syntropy::reflection::ClassDeclaration<type> { \
    auto operator()() const{ \
        return syntropy::reflection::Class::Definition<type>(#type); \
    } \
}

// Void

SYN_DEFINE_FUNDAMENTAL_TYPE(void);

// Null pointer

SYN_DEFINE_FUNDAMENTAL_TYPE(std::nullptr_t);

// Boolean

SYN_DEFINE_FUNDAMENTAL_TYPE(bool);

// Character

SYN_DEFINE_FUNDAMENTAL_TYPE(char);

// Integer

SYN_DEFINE_FUNDAMENTAL_TYPE(int);

// Float

SYN_DEFINE_FUNDAMENTAL_TYPE(float);

#undef SYN_DEFINE_FUNDAMENTAL_TYPE      // Ugly macro, be gone! :D
