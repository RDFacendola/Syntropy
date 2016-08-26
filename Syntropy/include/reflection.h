
/// \file reflection.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <unordered_map>

#include "hashed_string.h"
#include "class.h"

namespace syntropy {

    namespace reflection {
        
        /// \brief Contains the list of all classes registered so far.
        /// \author Raffaele D. Facendola - 2016
        class Reflection {

            friend class Class;

        public:

            /// \brief Get the singleton instance.
            /// \return Returns the singleton instance.
            static Reflection& GetInstance() noexcept;

            /// \brief No copy constructor.
            Reflection(const Reflection&) = delete;

            /// \brief No assignment operator.
            Reflection& operator=(const Reflection&) = delete;

            /// \brief Get a class instance by name.
            /// \param class_name Name of the class to get.
            /// \return Returns a pointer to the class whose name is the specified one, if any. Returns nullptr otherwise.
            const Class* GetClass(const HashedString& class_name) noexcept;

        private:

            /// \brief Private constructor to prevent instantiation and inheritance.
            Reflection();

            /// \brief Register a new class to the registry.
            /// \param class_instance Class to register.
            void Register(Class& class_instance);

            std::unordered_map<size_t, Class*> classes_;       ///< \brief List of classes registered so far. The key is the class name's hash.

        };

    }

}

namespace syntropy {

    namespace reflection {

        // Implementation

        //////////////// REFLECTION ////////////////

        inline Reflection& Reflection::GetInstance() noexcept {

            static Reflection instance;

            return instance;

        }

    }

}