
/// \file class.h
/// \brief This header is part of the syntropy reflection system. It contains classes used to handle class definition.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <sstream>
#include <ostream>

#include "hashed_string.h"
#include "type_traits.h"

#include "instance.h"
#include "property.h"
#include "method.h"

namespace syntropy {

    namespace reflection {
        
        /// \brief Describes a class.
        /// A class can be used to access fields, properties and methods or eventually instantiate new objects.
        /// \remarks This class is a singleton.
        /// \author Raffaele D. Facendola - 2016
        class Class {

        public:

            template <typename TClass>
            class Definition;

            /// \brief Get the class associated to TClass.
            /// \return Returns a reference to the singleton describing the class TClass.
            template <typename TClass>
            static const Class& GetClass();

            /// \brief No copy constructor.
            Class(const Class&) = delete;

            /// \brief No assignment operator.
            Class& operator=(const Class&) = delete;

            /// \brief Virtual destructor.
            virtual ~Class() = default;

            /// \brief Equality comparison.
            /// Check whether this class is or derives from the specified one.
            /// \return Returns true if this class is or derives from the specified class, return false otherwise.
            /// \remarks This method accounts for inheritance by not for implicit conversion. For example Class(float) == Class(int) is false even if such conversion exists.
            bool operator==(const Class& other) const noexcept;

            /// \brief Inequality comparison.
            /// Check whether the class described by this instance doesn't derive from the specified class.
            /// \return Returns true if this class doesn't derive from the specified one, returns false otherwise.
            bool operator!=(const Class& other) const noexcept;

            /// \brief Get the default class name.
            /// \return Returns the default class name.
            virtual const HashedString& GetName() const noexcept = 0;

            /// \brief Get all the class name aliases.
            /// Certain types have different name aliases (like signed short int, short, short int which refers to the same type).
            /// The first element in the list is guaranteed to be the default class name.
            /// \brief Returns a list containing the class names.
            virtual const std::vector<HashedString>& GetNames() const noexcept = 0;

            /// \brief Get the list of the base classes of this class.
            /// \return Returns the list of classes that are extended by this class.
            virtual const std::vector<const Class*>& GetBaseClasses() const noexcept = 0;

            /// \brief Get a class property by name.
            /// \param property_name Name of the property to get.
            /// \return Returns a pointer to the requested property, if any. Returns nullptr otherwise.
            virtual const Property* GetProperty(const HashedString& property_name) const noexcept = 0;

            /// \brief Get the list of properties supported by this class.
            /// \return Returns the list of properties supported by this class.
            virtual const std::vector<Property>& GetProperties() const noexcept = 0;

            /// \brief Check whether this class is abstract or not.
            /// \return Returns true if the class is abstract, returns false otherwise.
            virtual bool IsAbstract() const noexcept = 0;

            /// \brief Check whether the class is instantiable via reflection.
            /// A class is instantiable if it is non-abstract and is default-constructible.
            /// \return Returns true if the class is instantiable, returns false otherwise.
            virtual bool IsInstantiable() const noexcept = 0;

            /// \brief Create a new instance.
            /// The method requires that IsInstantiable() is true, that is the class is both default-constructible and concrete.
            /// \return Returns a reference to the new instance if the class was instantiable, return an empty reference otherwise.
            virtual Instance Instantiate() const = 0;

        private:
            
            template <typename TClass>
            class ClassT;

            /// \brief Default constructor.
            Class() = default;

        };

        /// \brief Concrete class definition.
        /// This class is used to define class names, properties and methods.
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass>
        class Class::Definition {

            friend class Class::ClassT<TClass>;

        public:

            /// \brief Create a new class definition.
            /// \param default_name Default class name.
            Definition(std::string default_name);

            /// \brief Default copy constructor.
            /// \param other Instance to copy.
            Definition(const Definition& other) = default;

            /// \brief Move constructor.
            /// \param other Instance to move.
            Definition(Definition&& other);

            /// \brief Default assignment operator.
            /// \param other Instance to copy.
            Definition& operator=(const Definition& other) = default;

            /// \brief Define a name alias for the class.
            /// If the name was already defined this method does nothing.
            /// \param name Name alias.
            void DefineNameAlias(HashedString name) noexcept;

            /// \brief Define a base class.
            /// If the base class was already defined this method does nothing.
            /// \tparam TBaseClass Type of the base class.
            template <typename TBaseClass>
            void DefineBaseClass() noexcept;

            /// \brief Define a class property.
            /// This overload accounts for fields and read-only getters.
            /// 
            /// Examples:
            /// d.DefineProperty("foo", &Foo::field)        // Defines a read\write property from a member variable.
            /// d.DefineProperty("Foo", &Foo::GetField)     // Defines a read-only property from a const getter.
            template <typename TProperty>
            void DefineProperty(const HashedString& name, TProperty&& property);
            
            /// \brief Define a class property.
            /// This overload accounts for properties that are accessed via getters and setters.
            ///
            /// Examples:
            /// d.DefineProperty("Foo", &Foo::GetFoo, &Foo::SetFoo)     // Getter of the form "const Foo& GetFoo() const", setter of the form "void SetFoo(const Foo&)"
            /// d.DefineProperty("Foo", &Foo::GetFoo, &Foo::AccessFoo)  // Getter of the form "const Foo& GetFoo() const", accessor of the form "Foo& AccessFoo"
            template <typename TGetter, typename TSetter>
            void DefineProperty(const HashedString& name, TGetter&& getter, TSetter&& setter);

        private:

            void CheckPropertyNameOrDie(const HashedString& property_name) const;

            std::vector<HashedString> names_;                   ///< \brief Class names.

            std::vector<const Class*> base_classes_;            ///< \brief List of all base classes.

            std::vector<Property> properties_;                  ///< \brief Reference to the class properties.

        };

        /// \brief Describes an actual class.
        /// Used to perform type erasure.
        /// \author Raffaele D. Facendola - August 2016
        template <typename TClass>
        class Class::ClassT : public Class {

        public:

            /// \brief Create a new class description via class definition.
            ClassT(Definition<TClass> definition);

            virtual const HashedString& GetName() const noexcept override;

            virtual const std::vector<HashedString>& GetNames() const noexcept override;

            virtual const std::vector<const Class*>& GetBaseClasses() const noexcept override;

            virtual const Property* GetProperty(const HashedString& property_name) const noexcept override;

            virtual const std::vector<Property>& GetProperties() const noexcept override;

            virtual bool IsAbstract() const noexcept override;

            virtual bool IsInstantiable() const noexcept override;

            virtual Instance Instantiate() const override;

        private:

            using TPropertyHash = std::hash<HashedString>::result_type;

            /// \brief Regenerates the indices used to access properties and methods by name.
            void RegenerateIndices() noexcept;

            std::vector<HashedString> names_;                                       ///< \brief Class names.
            
            std::vector<const Class*> base_classes_;                                ///< \brief List of base classes.

            std::vector<Property> properties_;                                      ///< \brief List of class properties.
            
            std::unordered_map<TPropertyHash, const Property*> properties_index_;   ///< \brief Index over the class properties used to access properties by name.

        };

        /// \brief Utility method used to get a class by type.
        /// \return Returns a reference to the class describing TType.
        template <typename TType>
        const Class& ClassOf();

        /// \brief Utility method used to get a class by type.
        /// \return Returns a reference to the class describing TType.
        template <typename TType>
        const Class& ClassOf(TType&&);

        /// \brief Utility method used to get a class definition by type.
        /// \return Returns the concrete definition of the class TClass.
        template <typename TClass>
        Class::Definition<TClass> DefinitionOf();

        /// \brief Functor used to fill a class definition.
        /// Specialize this functor for each class requiring reflection.
        template <typename TClass>
        struct ClassDeclaration;

        /// \brief Stream insertion for Class.
        std::ostream& operator<<(std::ostream& out, const Class& class_instance);

    }

}

namespace syntropy {

    namespace reflection {

        // Implementation

        //////////////// CLASS ////////////////

        template <typename TClass>
        inline static const Class& Class::GetClass() {

            static_assert(is_class_name_v<TClass>, "TClass must be a plain class name (without pointers, references, extents and/or qualifiers)");

            static ClassT<TClass> instance(DefinitionOf<TClass>());

            return instance;

        }

        //////////////// CLASS :: DEFINITION ////////////////

        template <typename TClass>
        inline Class::Definition<TClass>::Definition(std::string default_name) {

            names_.push_back(std::move(default_name));

        }

        template <typename TClass>
        inline Class::Definition<TClass>::Definition(Definition<TClass>&& other)
            : names_(std::move(other.names_))
            , base_classes_(std::move(other.base_classes_))
            , properties_(std::move(other.properties_)) {}

        template <typename TClass>
        inline void Class::Definition<TClass>::DefineNameAlias(HashedString name) noexcept {

            auto it = std::find_if(names_.begin(), names_.end(), [&name](const HashedString& class_name) { return class_name == name; });

            if (it == names_.end()) {
            
                names_.push_back(name);

            }
            
        }

        template <typename TClass>
        template <typename TBaseClass>
        inline void Class::Definition<TClass>::DefineBaseClass() noexcept {

            static_assert(std::is_base_of_v<TBaseClass, TClass>, "The class being defined does not derive from TBaseClass");
            static_assert(!std::is_same<TBaseClass, TClass>::value, "A class cannot derive from itself");

            auto base_class = &ClassOf<TBaseClass>();

            auto it = std::find(base_classes_.begin(), base_classes_.end(), base_class);

            if (it == base_classes_.end()) {

                base_classes_.push_back(base_class);

            }
            
        }

        template <typename TClass>
        template <typename TProperty>
        void Class::Definition<TClass>::DefineProperty(const HashedString& name, TProperty&& property) {

            CheckPropertyNameOrDie(name);
            
            properties_.emplace_back(name, 
                                     std::forward<TProperty>(property));

        }

        template <typename TClass>
        template <typename TGetter, typename TSetter>
        void Class::Definition<TClass>::DefineProperty(const HashedString& name, TGetter&& getter, TSetter&& setter) {

            CheckPropertyNameOrDie(name);

            properties_.emplace_back(name, 
                                     std::forward<TGetter>(getter),
                                     std::forward<TSetter>(setter));

        }

        template <typename TClass>
        void Class::Definition<TClass>::CheckPropertyNameOrDie(const HashedString& property_name) const {

            auto it = std::find_if(properties_.begin(), properties_.end(), [&property_name](const Property& property) { return property.GetName() == property_name; });

            if (it != properties_.end()) {

                std::stringstream ss;

                ss << "A property named '" << property_name << "' already exists.";

                throw std::invalid_argument(ss.str());

            }

        }

        //////////////// CLASS :: CLASS T ////////////////

        template <typename TClass>
        Class::ClassT<TClass>::ClassT(Definition<TClass> definition)
            : names_(std::move(definition.names_))
            , base_classes_(std::move(definition.base_classes_))
            , properties_(std::move(definition.properties_)){

            RegenerateIndices();

            Reflection::GetInstance().Register(*this);

        }

        template <typename TClass>
        inline const HashedString& Class::ClassT<TClass>::GetName() const noexcept {

            return names_[0];

        }

        template <typename TClass>
        inline const std::vector<HashedString>& Class::ClassT<TClass>::GetNames() const noexcept {

            return names_;

        }

        template <typename TClass>
        inline const std::vector<const Class*>& Class::ClassT<TClass>::GetBaseClasses() const noexcept {

            return base_classes_;

        }

        template <typename TClass>
        inline const Property* Class::ClassT<TClass>::GetProperty(const HashedString& property_name) const noexcept {

            auto it = properties_index_.find(std::hash<HashedString>()(property_name));

            return it != properties_index_.end() ?
                   it->second :
                   nullptr;

        }

        template <typename TClass>
        inline const std::vector<Property>& Class::ClassT<TClass>::GetProperties() const noexcept {

            return properties_;

        }

        template <typename TClass>
        inline bool Class::ClassT<TClass>::IsAbstract() const noexcept {

            return std::is_abstract_v<TClass>;

        }

        template <typename TClass>
        inline bool Class::ClassT<TClass>::IsInstantiable() const noexcept {

            return std::is_default_constructible_v<TClass>;

        }

        template <typename TClass>
        inline Instance Class::ClassT<TClass>::Instantiate() const {

            return instantiate<TClass>()();

        }

        template <typename TClass>
        inline void Class::ClassT<TClass>::RegenerateIndices() noexcept{

            properties_index_.clear();
            properties_index_.reserve(properties_.size());

            std::transform(properties_.begin(), properties_.end(),
                           std::inserter(properties_index_, properties_index_.begin()),
                           [](const Property& property) {

                                return std::make_pair(std::hash<HashedString>()(property.GetName()),
                                                      &property);

                           });

        }

        //////////////// CLASS OF ////////////////

        template <typename TType>
        const Class& ClassOf() {

            return Class::GetClass<class_name_t<TType>>();

        }

        template <typename TType>
        const Class& ClassOf(TType&&) {

            return Class::GetClass<class_name_t<TType&&>>();

        }

        //////////////// DEFINITION OF ////////////////

        template <typename TClass>
        Class::Definition<TClass> DefinitionOf() {

            static_assert(is_class_name_v<TClass>, "TClass must be a plain class name.");

            ClassDeclaration<TClass> declaration;

            Class::Definition<TClass> definition(declaration.GetName());

            TryCall(declaration, definition);   // Filling the definition is optional.

            return definition;

        }

    }

}