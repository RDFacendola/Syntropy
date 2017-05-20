
/// \file reflection.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "containers/hashed_string.h"

#include "diagnostics/diagnostics.h"

#include <unordered_map>

namespace syntropy
{

    namespace reflection
    {
     
        /// \brief Log context for the syntropy reflection system.
        extern const diagnostics::Context ReflectionCtx;

        class Class;

        /// \brief Contains the list of all classes registered so far.
        /// \author Raffaele D. Facendola - 2016
        class Reflection
        {

        public:

            /// \brief Get the singleton instance.
            /// \return Returns the singleton instance.
            static Reflection& GetInstance() noexcept;

            /// \brief No copy constructor.
            Reflection(const Reflection&) = delete;

            /// \brief No assignment operator.
            Reflection& operator=(const Reflection&) = delete;

            /// \brief Get a class instance by name.
            /// A default class name always takes precedence over aliases.
            /// \param class_name Name or alias of the class to get.
            /// \return Returns a pointer to the class whose name is the specified one, if any. Returns nullptr otherwise.
            const Class* GetClass(const HashedString& class_name) const noexcept;

            /// \brief Register a new class to the registry.
            /// \param class_instance Class to register.
            void Register(Class& class_instance);

        private:

            /// \brief Private constructor to prevent instantiation and inheritance.
            Reflection();

            std::unordered_map<HashedString, Class*> default_classes_;          ///< \brief List of classes registered so far. Default class names only.

            std::unordered_map<HashedString, Class*> class_aliases_;            ///< \brief List of class aliases registered so far.
        };

        /// \brief Get a class instance by name.
        /// \param class_name Name or alias of the class to get.
        /// \return Returns a pointer to the class whose name is the specified one, if any. Returns nullptr otherwise.
        const Class* GetClass(const HashedString& class_name) noexcept;

    }

}