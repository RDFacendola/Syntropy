
/// \file class.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <type_traits>
#include <ostream>

#include "hashed_string.h"
#include "type_traits.h"

#include "instance.h"
#include "property.h"
#include "method.h"

namespace syntropy {

    namespace reflection {
        
        class IClassDefinition;

        /// \brief Describes a class implementation.
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
            static const Class& GetClass();

            /// \brief Get the name of the class.
            /// \return Returns the type string of the class.
            const HashedString& GetName() const noexcept;

            /// \brief Get the list of classes that are derived by this class.
            /// \return Returns the list of classes that are derived by this class.
            const std::vector<const Class*>& GetBaseClasses() const noexcept;

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

            /// \brief Check whether this class is abstract or not.
            /// \return Returns true if the class is abstract, returns false otherwise.
            bool IsAbstract() const noexcept;

            /// \brief Check whether the class is instantiable via reflection.
            /// A class is instantiable if it is non-abstract and is default-constructible.
            /// \return Returns true if the class is instantiable, returns false otherwise.
            bool IsInstantiable() const noexcept;

            /// \brief Check whether this class can be converted to the specified one.
            /// \return Returns true if the class described by this instance is a base for the specified one or is the same type, returns false otherwise.
            bool operator==(const Class& other) const noexcept;

            /// \brief Check whether this class cannot be converted to the specified one.
            /// \return Returns false if the class described by this instance is a base for the specified one or is the same type, returns true otherwise.
            bool operator!=(const Class& other) const noexcept;

            /// \brief Create a new instance.
            /// The method requires that the class is default-constructible and is non-abstract.
            /// \return Returns a reference to the new instance if the class was default constructible, return an empty reference otherwise.
            Instance Instantiate() const;

        private:
            
            /// \brief Create a new class via explicit class definition.
            Class(std::unique_ptr<IClassDefinition> definition);
            
            size_t class_id_;                                       ///< \brief Unique id. 
                                                                    ///< \remarks Can be different at each execution.

            std::unique_ptr<IClassDefinition> definition_;			///< \brief Class definition.


        };

        template <typename TType>
        const Class& ClassOf();

        /// \brief Interface for class definition.
        /// \author Raffaele D. Facendola - 2016
        class IClassDefinition {

        public:

            /// \brief Virtual destructor.
            virtual ~IClassDefinition() = default;
            
            /// \brief Get the name of the class.
            /// \return Returns the type string of the class.
            virtual const HashedString& GetName() const noexcept = 0;

            /// \brief Get the list of classes that are derived by this class.
            /// \return Returns the list of classes that are derived by this class.
            virtual const std::vector<const Class*>& GetBaseClasses() const noexcept = 0;
            
            /// \brief Get a class property by name.
            /// \param property_name Name of the property to get.
            /// \return Returns a pointer to the requested property, if any. Returns nullptr otherwise.
            virtual const Property* GetProperty(const HashedString& property_name) const noexcept = 0;

            /// \brief Get a class method by name.
            /// \param method_name Name of the method to get.
            /// \return Returns a pointer tot he requested method if any. Returns nullptr otherwise.
            virtual const Method* GetMethod(const HashedString& method_name) const noexcept = 0;

            /// \brief Get the class properties list.
            /// \return Returns the class properties list.
            virtual const std::unordered_map<size_t, Property>& GetProperties() const noexcept = 0;

            /// \brief Check whether this class is abstract or not.
            /// \return Returns true if the class is abstract, returns false otherwise.
            virtual bool IsAbstract() const noexcept = 0;

            /// \brief Check whether the class is instantiable via reflection.
            /// A class is instantiable if it is non-abstract and is default-constructible.
            /// \return Returns true if the class is instantiable, returns false otherwise.
            virtual bool IsInstantiable() const noexcept = 0;

            /// \brief Create a new instance.
            /// The method requires that the class is default-constructible and is non-abstract.
            /// \return Returns a reference to the new instance if the class was default constructible, return an empty reference otherwise.
            virtual Instance Instantiate() const = 0;

        };

        /// \brief Concrete class definition.
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass>
        class ClassDefinition : public IClassDefinition {

        public:

            /// \brief No copy constructor.
            ClassDefinition(const ClassDefinition<TClass>& other) = delete;

            /// \brief No assignment operator.
            ClassDefinition<TClass>& operator=(const ClassDefinition<TClass>& other) = delete;

            /// \brief Create a named class definition.
            /// \param name name of the class.
            ClassDefinition(const HashedString& name) noexcept;

            virtual const HashedString& GetName() const noexcept override;

            virtual const std::vector<const Class*>& GetBaseClasses() const noexcept override;

            virtual const Property* GetProperty(const HashedString& property_name) const noexcept override;

            virtual const Method* GetMethod(const HashedString& method_name) const noexcept override;

            virtual const std::unordered_map<size_t, Property>& GetProperties() const noexcept override;

            virtual bool IsAbstract() const noexcept override;

            virtual bool IsInstantiable() const noexcept override;

            virtual Instance Instantiate() const override;

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

            HashedString name_;                                     ///< \brief Unique name.

            std::vector<const Class*> base_classes_;                ///< \brief List of all classes that are base of this one.

            std::unordered_map<size_t, Property> properties_;       ///< \brief List of all the supported properties.

            std::unordered_map<size_t, Method> methods_;            ///< \brief List of all the supported methods.

        };

        /// \brief Functor used to fill a class definition.
        /// Specialize this functor for each class requiring reflection.
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass>
        struct ClassDeclaration;

        /// \brief Stream insertion for Class.
        std::ostream& operator<<(std::ostream& out, const Class& class_instance);

    }

    template <typename TInstance>
    struct class_get<reflection::Class, TInstance> {

        const reflection::Class& operator()() const noexcept {

            return reflection::ClassOf<TInstance>();

        }

    };

}

namespace syntropy {

    namespace reflection {

        // Implementation

        //////////////// CLASS ////////////////

        template <typename TClass>
        inline static const Class& Class::GetClass() {

            static_assert(is_class_name_v<TClass>, "TClass must be a plain class name (without pointers, references, extents and/or qualifiers)");

            static Class instance(ClassDeclaration<TClass>{}());

            return instance;
            
        }

        inline const HashedString& Class::GetName() const noexcept {

            return definition_->GetName();

        }
    
        inline const std::vector<const Class*>& Class::GetBaseClasses() const noexcept {

            return definition_->GetBaseClasses();

        }

        inline const Property* Class::GetProperty(const HashedString& property_name) const noexcept {

            return definition_->GetProperty(property_name);
                
        }

        inline const Method* Class::GetMethod(const HashedString& method_name) const noexcept {

            return definition_->GetMethod(method_name);

        }

        inline const std::unordered_map<size_t, Property>& Class::GetProperties() const noexcept {

            return definition_->GetProperties();

        }

        inline bool Class::IsAbstract() const noexcept {

            return definition_->IsAbstract();

        }
        
        inline bool Class::IsInstantiable() const noexcept {

            return definition_->IsInstantiable();

        }
        
        inline Instance Class::Instantiate() const {

            return definition_->Instantiate();

        }

        //////////////// CLASS OF ////////////////

        template <typename TType>
        const Class& ClassOf() {

            return Class::GetClass<class_name_t<TType>>();

        }

        //////////////// CLASS DEFINITION ////////////////

        template <typename TClass>
        inline ClassDefinition<TClass>::ClassDefinition(const HashedString& name) noexcept
            : name_(name) {}

        template <typename TClass>
        inline const HashedString& ClassDefinition<TClass>::GetName() const noexcept {

            return name_;

        }
    
        template <typename TClass>
        inline const std::vector<const Class*>& ClassDefinition<TClass>::GetBaseClasses() const noexcept {

            return base_classes_;

        }

        template <typename TClass>
        inline const Property* ClassDefinition<TClass>::GetProperty(const HashedString& property_name) const noexcept {

            auto it = properties_.find(std::hash<HashedString>()(property_name));

            return it != properties_.end() ?
                   std::addressof(it->second) :
                   nullptr;
                
        }

        template <typename TClass>
        inline const Method* ClassDefinition<TClass>::GetMethod(const HashedString& method_name) const noexcept {

            auto it = methods_.find(std::hash<HashedString>()(method_name));

            return it != methods_.end() ?
                   std::addressof(it->second) :
                   nullptr;
        
        }

        template <typename TClass>
        inline const std::unordered_map<size_t, Property>& ClassDefinition<TClass>::GetProperties() const noexcept {

            return properties_;

        }

        template <typename TClass>
        inline bool ClassDefinition<TClass>::IsAbstract() const noexcept {

            return std::is_abstract_v<TClass>;

        }

        template <typename TClass>
        inline bool ClassDefinition<TClass>::IsInstantiable() const noexcept {

            return std::is_default_constructible_v<TClass>;
            
        }

        template <typename TClass>
        inline Instance ClassDefinition<TClass>::Instantiate() const {

            return instantiate<TClass>()();

        }
        
        template <typename TClass>
        template <typename TBaseClass>
        inline void ClassDefinition<TClass>::DefineBaseClass() noexcept {

            static_assert(std::is_base_of_v<TBaseClass, TClass>, "The class being defined does not derive from TBaseClass");
            static_assert(!std::is_same<TBaseClass, TClass>::value, "A class cannot derive from itself");

            base_classes_.push_back(std::addressof(ClassOf<TBaseClass>()));

        }

        template <typename TClass>
        template <typename TProperty>
        void ClassDefinition<TClass>::DefineProperty(const HashedString& name, TProperty&& property) noexcept {

            properties_.emplace(std::make_pair(std::hash<HashedString>()(name),
                                               Property(name, 
                                                        std::forward<TProperty>(property))));

        }

        template <typename TClass>
        template <typename TGetter, typename TSetter>
        void ClassDefinition<TClass>::DefineProperty(const HashedString& name, TGetter&& getter, TSetter&& setter) noexcept {

            properties_.emplace(std::make_pair(std::hash<HashedString>()(name),
                                               Property(name, 
                                                        std::forward<TGetter>(getter),
                                                        std::forward<TSetter>(setter))));

        }
     
        //////////////// STREAM INSERTION ////////////////

        inline std::ostream& operator<<(std::ostream& out, const Class& class_instance) {

            out << class_instance.GetName().GetString();

            return out;

        }

    }

}