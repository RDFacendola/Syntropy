
/// \file method.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "hashed_string.h"

namespace syntropy {

    namespace reflection {

        class Method {

        public:

            const HashedString& GetName() const noexcept;
       
        private:

            HashedString name_;

        };

        //////////////// METHOD ////////////////

        inline const HashedString& Method::GetName() const noexcept {

            return name_;

        }


    }

}

namespace std {

    /// \brief Custom template specialization of std::hash for syntropy::reflection::Method.
    /// \author Raffaele D. Facendola - 2016
    template <>
    struct hash<syntropy::reflection::Method> {

        using result_type = size_t;

        using argument_type = syntropy::reflection::Method;

        result_type operator()(const argument_type& argument) {

            return argument.GetName().GetHash();

        }

    };

}



