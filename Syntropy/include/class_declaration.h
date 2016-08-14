
/// \file class_declaration.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <unordered_map>

#include "property.h"
#include "method.h"

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
        
        /// \brief Interface for class declarations.
        /// \author Raffaele D. Facendola - 2016
        class IClassDeclaration {

        public:

            /// \brief Virtual destructor.
            virtual ~IClassDeclaration() = default;

            /// \brief Get the name of the class.
            /// \return Returns the type string of the class.
            virtual const HashedString& GetName() const noexcept = 0;

            /// \brief Get the list of classes that are derived by this class.
            /// \return Returns the list of classes that are derived by this class.
            virtual const std::vector<Class*>& GetBaseClasses() const noexcept = 0;

            /// \brief Get the factory associated to the class.
            /// \return Returns a pointer to the class factory. Note that abstract classes have no factory since they cannot be instantiated.
            virtual const ClassFactory* GetFactory() const noexcept = 0;
            /// \brief Get a class property by name.
            /// \param property_name Name of the property to get.
            /// \return Returns a pointer to the requested property, if any. Returns nullptr otherwise.
            virtual const Property* GetProperty(const HashedString& property_name) const noexcept = 0;

            /// \brief Get a class method by name.
            /// \param method_name Name of the method to get.
            /// \return Returns a pointer to the requested method if any. Returns nullptr otherwise.
            virtual const Method* GetMethod(const HashedString& method_name) const noexcept = 0;

            /// \brief Get the class properties list.
            /// \return Returns the class properties list.
            virtual const std::unordered_map<size_t, Property>& GetProperties() const noexcept = 0;

        };

        /// \brief Concrete class declaration.
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass>
        class ClassDeclaration : public IClassDeclaration {

        public:

            ClassDeclaration() noexcept;
            
            ClassDeclaration(const HashedString& name) noexcept;

            /// \brief Virtual destructor.
            virtual ~ClassDeclaration() = default;

            virtual const HashedString& GetName() const noexcept override;

            virtual const std::vector<Class*>& GetBaseClasses() const noexcept override;

            virtual const ClassFactory* GetFactory() const noexcept override;

            virtual const Property* GetProperty(const HashedString& property_name) const noexcept override;

            virtual const Method* GetMethod(const HashedString& method_name) const noexcept override;

            virtual const std::unordered_map<size_t, Property>& GetProperties() const noexcept override;

            template <typename TBaseClass>
            void DefineBaseClass() noexcept;

            /// Properties and getter of the form
            /// TProperty TClass::*
            /// TProperty (TClass::*)() const
            template <typename TProperty>
            void DefineProperty(const HashedString& name, TProperty&& property) noexcept;

            /// Getters and setters of the form
            /// TProperty (TClass::*)() const / void (TClass::*)(TProperty)
            /// const TProperty& (TClass::*)() const / TProperty& (TClass::*)()
            template <typename TGetter, typename TSetter>
            void DefineProperty(const HashedString& name, TGetter&& getter, TSetter&& setter) noexcept;

            //template <typename TMethod>
            //void DefineMethod(const HashedString& method_name, TMethod&& method) noexcept;

        private:

            HashedString name_;

            std::vector<Class*> base_classes_;

            std::unordered_map<size_t, Property> properties_;

            std::unordered_map<size_t, Method> methods_;

        };

        /// \brief Contains the concrete definition of a class type.
        /// Specialize this template in the same compilation unit where access to the proper class is needed.
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass>
        class ClassDefinition {

        public:

            ClassDeclaration<TClass> operator()() const {

                return ClassDeclaration<TClass>();

            }

        };

        //////////////// CLASS DECLARATION ////////////////

        template <typename TClass>
        inline ClassDeclaration<TClass>::ClassDeclaration() noexcept
            : name_(typeid(TClass).name()) {}
            
        template <typename TClass>
        inline ClassDeclaration<TClass>::ClassDeclaration(const HashedString& name) noexcept
            : name_(name) {}

        template <typename TClass>
        inline const HashedString& ClassDeclaration<TClass>::GetName() const noexcept {

            return name_;

        }
    
        template <typename TClass>
        inline const std::vector<Class*>& ClassDeclaration<TClass>::GetBaseClasses() const noexcept {

            return base_classes_;

        }

        template <typename TClass>
        const ClassFactory* ClassDeclaration<TClass>::GetFactory() const noexcept {

            // TODO: implement me!
            return nullptr;

        }

        template <typename TClass>
        inline const Property* ClassDeclaration<TClass>::GetProperty(const HashedString& property_name) const noexcept {

            auto it = properties_.find(std::hash<HashedString>()(property_name));

            return it != properties_.end() ?
                   std::addressof(it->second) :
                   nullptr;
                
        }

        template <typename TClass>
        inline const Method* ClassDeclaration<TClass>::GetMethod(const HashedString& method_name) const noexcept {

            auto it = methods_.find(std::hash<HashedString>()(method_name));

            return it != methods_.end() ?
                   std::addressof(it->second) :
                   nullptr;
        
        }

        template <typename TClass>
        inline const std::unordered_map<size_t, Property>& ClassDeclaration<TClass>::GetProperties() const noexcept {

            return properties_;

        }

        template <typename TClass>
        template <typename TBaseClass>
        inline void ClassDeclaration<TClass>::DefineBaseClass() noexcept {

            static_assert(std::is_base_of_v<TBaseClass, TClass>, "The class being defined does not derive from TBaseClass");

            base_classes_.push_back(std::addressof(Class::GetClass<TBaseClass>()));

        }

        template <typename TClass>
        template <typename TProperty>
        void ClassDeclaration<TClass>::DefineProperty(const HashedString& name, TProperty&& property) noexcept {

            properties_.emplace(std::make_pair(std::hash<HashedString>()(name),
                                               Property(name, 
                                                        std::forward<TProperty>(property))));

        }

        template <typename TClass>
        template <typename TGetter, typename TSetter>
        void ClassDeclaration<TClass>::DefineProperty(const HashedString& name, TGetter&& getter, TSetter&& setter) noexcept {

            properties_.emplace(std::make_pair(std::hash<HashedString>()(name),
                                               Property(name, 
                                                        std::forward<TGetter>(getter),
                                                        std::forward<TSetter>(setter))));

        }

    }

}
