
/// \file utils.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "class.h"

namespace syntropy {

    namespace reflection {

        class Class;

        struct IClassProvider;

        template <typename TClass>
        struct ClassProvider;

    }

}

namespace syntropy {

    namespace reflection {

        struct IClassProvider {

            virtual const Class& operator()() const noexcept = 0;

        };

        template <typename TClass>
        struct ClassProvider : public IClassProvider {

            virtual const Class& operator()() const noexcept override;

        };

        //////////////// CLASS PROVIDER ////////////////

        template <typename TClass>
        inline const Class& ClassProvider<TClass>::operator()() const noexcept {

            return Class::GetClass<TClass>();

        }
        
    }

}