
/// \file class.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <unordered_map>
#include <memory>

#include "hashed_string.h"
#include "class_declaration.h"

namespace syntropy {

    namespace reflection {

        class IClassDeclaration;

        template <typename TClass>
        class ClassDeclaration;

        template <typename TClass>
        class ClassDefinition;
        
        class Class;
        class Property;
        class Method;

        class ClassRegistry;
        class ClassFactory;

    }

}

namespace syntropy {

    namespace reflection {

        /// \brief Describes a class type.
        /// \author Raffaele D. Facendola - 2016
        class Class {

        public:

            /// \brief No copy constructor.
            Class(const Class&) = delete;

            /// \brief No assignment operator.
            Class& operator=(const Class&) = delete;

            /// \brief Virtual destructor.
            virtual ~Class() = default;

            template <typename TClass>
            static Class& GetClass() noexcept;

            /// \brief Get the name of the class.
            /// \return Returns the type string of the class.
            const HashedString& GetName() const noexcept;

            /// \brief Get the list of classes that are derived by this class.
            /// \return Returns the list of classes that are derived by this class.
            const std::vector<Class*>& GetBaseClasses() const noexcept;

            /// \brief Get a factory for this class.
            /// \return Returns an factory for this class if applicable. Returns nullptr otherwise.
            const ClassFactory* GetFactory() const noexcept;

            /// \brief Get a class property by name.
            /// \param property_name Name of the property to get.
            /// \return Returns a pointer to the requested property, if any. Returns nullptr otherwise.
            const Property* GetProperty(const HashedString& property_name) const noexcept;

            /// \brief Get a class method by name.
            /// \param method_name Name of the method to get.
            /// \return Returns a pointer tot he requested method if any. Returns nullptr otherwise.
            const Method* GetMethod(const HashedString& method_name) const noexcept;

            /// \brief Get the class properties list.
            /// \return Returns the class properties list.
            const std::unordered_map<size_t, Property>& GetProperties() const noexcept;

            /// \brief Check whether this class derives from the specified one.
            /// A class is convertible if it is the same type or derives from another one.
            /// \return Returns true if the class is convertible to the specified type, return false otherwise.
            bool IsDerived(const Class& other) const noexcept;

        private:

            /// \brief Create a new class description.
            /// \param declaration Declaration of the class.
            Class(std::unique_ptr<IClassDeclaration> declaration);

            size_t class_id_;                                       ///< \brief Unique id of the class.

            std::unique_ptr<IClassDeclaration> declaration_;        ///< \brief Declaration of the class.

        };

        /// \brief Contains the list of all classes registered so far.
        /// \author Raffaele D. Facendola - 2016
        class ClassRegistry {

            friend class Class;

        public:

            /// \brief Get the singleton instance.
            /// \return Returns the singleton instance.
            static ClassRegistry& GetInstance() noexcept;

            /// \brief No copy constructor.
            ClassRegistry(const ClassRegistry&) = delete;
            
            /// \brief No assignment operator.
            ClassRegistry& operator=(const ClassRegistry&) = delete;
            
            /// \brief Get a class instance by name.
            /// \param class_name Name of the class to get.
            /// \return Returns a pointer to the class whose name is the specified one, if any. Returns nullptr otherwise.
            const Class* GetClass(const HashedString& class_name) noexcept;

        private:

            /// \brief Private constructor to prevent instantiation and inheritance.
            ClassRegistry();

            /// \brief Register a new class to the registry.
            /// \param class_instance Class to register.
            void Register(Class& class_instance);
            
            std::unordered_map<size_t, Class*> classes_;       ///< \brief List of classes registered so far.

        };

        //////////////// CLASS ////////////////

        template <typename TClass>
        inline static Class& Class::GetClass() noexcept {

            static_assert(std::is_same_v<decltype(std::declval<ClassDefinition<TClass>>()()),
                                                               ClassDeclaration<TClass>>,
                          "syntropy::reflection::ClassDefinition<TClass>() is expected to return a syntropy::reflection::ClassDeclaration<TClass>");

            static Class class_instance(std::make_unique<ClassDeclaration<TClass>>(ClassDefinition<TClass>{}()));

            return class_instance;

        }

        inline const HashedString& Class::GetName() const noexcept {

            return declaration_->GetName();

        }

        inline const std::vector<Class*>& Class::GetBaseClasses() const noexcept {

            return declaration_->GetBaseClasses();

        }

        inline const ClassFactory* Class::GetFactory() const noexcept {

            return declaration_->GetFactory();

        }

        inline const Property* Class::GetProperty(const HashedString& property_name) const noexcept {

            return declaration_->GetProperty(property_name);

        }

        inline const Method* Class::GetMethod(const HashedString& method_name) const noexcept {

            return declaration_->GetMethod(method_name);

        }

        inline const std::unordered_map<size_t, Property>& Class::GetProperties() const noexcept {

            return declaration_->GetProperties();

        }
        
        //////////////// CLASS REGISTRY ////////////////

        inline ClassRegistry& ClassRegistry::GetInstance() noexcept {

            static ClassRegistry instance;

            return instance;

        }

    }

}

namespace std {

    /// \brief Custom template specialization of std::hash for syntropy::reflection::Class.
    /// \author Raffaele D. Facendola - 2016
    template <>
    struct hash<syntropy::reflection::Class> {

        using result_type = size_t;

        using argument_type = syntropy::reflection::Class;

        result_type operator()(const argument_type& argument) {

            return argument.GetName().GetHash();

        }

    };

}