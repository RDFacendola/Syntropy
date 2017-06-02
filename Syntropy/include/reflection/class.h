
/// \file class.h
/// \brief This header is part of the syntropy reflection system. It contains classes used to handle class definition.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <vector>
#include <algorithm>

#include "cpp17.h"
#include "type_traits.h"
#include "utility.h"

#include "containers/hashed_string.h"
#include "containers/interface_container.h"

#include "reflection/type.h"
#include "reflection/property.h"
#include "reflection/interfaces/class_interfaces.h"

#include "diagnostics/log.h"

namespace syntropy
{
    namespace reflection
    {

        /// \brief Describes a class.
        /// A class can be used to access fields, properties and methods.
        /// \remarks This class is a singleton.
        /// \author Raffaele D. Facendola - 2016
        class Class
        {

            template <typename TClass>
            friend class ClassDefinitionT;

        public:

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
            /// Check whether this class is exactly the same as another class.
            /// \brief Returns true if this class is the same as the other class, returns false otherwise.
            bool operator==(const Class& other) const noexcept;

            /// \brief Inequality comparison.
            /// Check whether this class is not the same as another class.
            /// \brief Returns true if this class is not the same as the other class, returns false otherwise.
            bool operator!=(const Class& other) const noexcept;

            /// \brief Check whether this class is equal to or derives from another class.
            /// \return Returns true if this class is equal to or derives from other, returns false otherwise.
            bool IsA(const Class& other) const noexcept;

            /// \brief Get the default class name.
            /// \return Returns the default class name.
            const HashedString& GetDefaultName() const noexcept;

            /// \brief Get all the class name aliases except for the default class name.
            /// Certain types have different name aliases (like signed short int, short, short int which refer to the same type).
            /// \brief Returns a list containing the class names.
            const std::vector<HashedString>& GetNameAliases() const noexcept;

            /// \brief Get the list of the base classes of this class.
            /// \return Returns the list of classes that are extended by this class.
            const std::vector<const Class*>& GetBaseClasses() const noexcept;

            /// \brief Get a class property by name.
            /// Only properties defined in this class are checked.
            /// \return Returns a pointer to the requested property. If no such property could be found returns nullptr.
            const Property* GetProperty(const HashedString& property_name) const noexcept;

            /// \brief Get the list of properties supported by this class.
            /// \return Returns the list of properties supported by this class.
            const std::vector<Property>& GetProperties() const noexcept;

            /// \brief Check whether this class is abstract or not.
            /// \return Returns true if the class is abstract, returns false otherwise.
            bool IsAbstract() const noexcept;

            /// \brief Query the class for an interface of type TInterface.
            /// Only interfaces defined in this class are checked.
            /// \return If an interface of type TInterface was added during class declaration, returns a pointer to that interface, otherwise returns nullptr.
            /// \remarks This method doesn't account for polymorphism. If a class of type Foo derived from Bar is added to the class, GetInterface<Bar>() will return nullptr even if a conversion exists.
            template <typename TInterface>
            const TInterface* GetInterface() const;

        private:

            /// \brief Create a new class.
            template <typename TClass>
            Class(tag_t<TClass>);

            /// \brief Create a new class for the void type.
            Class(tag_t<void>);

            HashedString default_name_;                     ///< \brief Default class name.

            std::vector<HashedString> name_aliases_;        ///< \brief Class name aliases.

            std::vector<const Class*> base_classes_;        ///< \brief List of all base classes.

            std::vector<Property> properties_;              ///< \brief Class properties.

            InterfaceContainer<> interfaces_;               ///< \brief Interfaces assigned to this class.

            bool is_abstract_;                              ///< \brief Whether the class is abstract or not.

        };

        /// \brief Utility method used to get a class by type.
        /// \return Returns a reference to the class describing TType.
        template <typename TType>
        const Class& ClassOf();

        /// \brief Utility method used to get a class by type.
        /// \return Returns a reference to the class describing TType.
        template <typename TType>
        const Class& ClassOf(TType&&);

        /// \brief Stream insertion for Class.
        std::ostream& operator<<(std::ostream& out, const Class& class_instance);

        /// \brief Concrete class definition.
        /// This class is used to define class names, properties and methods.
        /// \author Raffaele D. Facendola - 2016
        template <typename TClass>
        class ClassDefinitionT
        {
            static_assert(is_class_name_v<TClass>, "TClass must be a plain class name (without pointers, references, extents and/or qualifiers)");

        public:

            /// \brief Create a new class definition.
            /// \param subject Class this definition refers to.
            ClassDefinitionT(Class& subject);

            /// \brief No copy constructor.
            ClassDefinitionT(const ClassDefinitionT&) = delete;

            /// \brief No assignment operator.
            ClassDefinitionT& operator=(const ClassDefinitionT&) = delete;

            /// \brief Default destructor.
            ~ClassDefinitionT() = default;

            /// \brief Define a name alias for the class.
            /// If the name was already defined this method does nothing.
            /// \param name Name alias.
            ClassDefinitionT& DefineNameAlias(const HashedString& name_alias) noexcept;

            /// \brief Define a base class.
            /// If the base class was already defined this method does nothing.
            /// \tparam TBaseClass Type of the base class.
            template <typename TBaseClass>
            ClassDefinitionT& DefineBaseClass() noexcept;

            /// \brief Define a class property.
            /// \param name Unique name of the class property.
            /// \param accessors Member field or methods used to access the property being defined.
            /// \return Returns the property definition.
            template <typename... TAccessors>
            PropertyDefinitionT<TAccessors...> DefineProperty(const HashedString& property_name, TAccessors... accessors);

            /// \brief Add a new interface to the class.
            /// The method creates an instance of TConcrete using TArgs as construction parameters. Only one interface of type TInterface can be added per class.
            /// TConcrete must be equal to or derive from TInterface.
            /// \param arguments Arguments to pass to the constructor of TInterface.
            template <typename TInterface, typename TConcrete = TInterface, typename... TArgs>
            ClassDefinitionT& AddInterface(TArgs&&... arguments);

            /// \brief Apply a functor to this class definition.
            template <typename TFunctor>
            ClassDefinitionT& operator<<(TFunctor&& functor);

        private:

            Class& subject_;            ///< \brief Class this definition refers to.

        };

        /// \brief Functor used to fill a class definition.
        /// Specialize this functor for each class requiring reflection.
        template <typename TClass>
        struct ClassDeclaration
        {
            // static constexpr const char* GetName();

            // void operator()(TDefinition& definition) const;
        };

        /// \brief Functor used to generate a complete class name.
        /// \tparam TArgument Template argument types (if any).
        /// \author Raffaele D. Facendola - May 2017
        template <typename... TArguments>
        struct GenerateClassNameT
        {
            std::string operator()(const char* class_name) const;
        };

        /// Utility object for GenerateClassNameT.
        template <typename... TArguments>
        constexpr GenerateClassNameT<TArguments...> GenerateClassName{};

    }

}

// Implementation

namespace syntropy 
{
    namespace reflection 
    {

        /************************************************************************/
        /* CLASS                                                                */
        /************************************************************************/

        template <typename TClass>
        static const Class& Class::GetClass()
        {
            static_assert(is_class_name_v<TClass>, "TClass must be a plain class name (without pointers, references, extents and/or qualifiers)");

            static Class instance(tag<TClass>);

            return instance;
        }

        template <typename TClass>
        Class::Class(tag_t<TClass>)
            : default_name_(ClassDeclaration<TClass>::GetName())
            , is_abstract_(std::is_abstract<TClass>::value)
        {
            static_assert(is_class_name_v<TClass>, "TClass must be a plain class name (without pointers, references, extents and/or qualifiers)");

            // Add common interfaces
            ConditionalAddInterface<std::is_default_constructible<TClass>::value, Constructible<>>(interfaces_, tag_t<TClass>{});

            // Fill base classes, properties, methods and name aliases via an explicit class declaration. Optional.
            ClassDefinitionT<TClass> definition(*this);

            conditional_call(ClassDeclaration<TClass>{}, definition);

            // Register the class to the reflection system.
            Reflection::GetInstance().Register(*this);
        }

        template <typename TInterface>
        const TInterface* Class::GetInterface() const
        {
            return interfaces_.GetInterface<TInterface>();
        }

        template <typename TType>
        const Class& ClassOf()
        {
            return Class::GetClass<class_name_t<TType>>();
        }

        template <typename TType>
        const Class& ClassOf(TType&&)
        {
            return Class::GetClass<class_name_t<TType&&>>();
        }

        /************************************************************************/
        /* CLASS DEFINITION T <TCLASS>                                          */
        /************************************************************************/

        template <typename TClass>
        ClassDefinitionT<TClass>::ClassDefinitionT(Class& subject)
            : subject_(subject)
        {

        }

        template <typename TClass>
        ClassDefinitionT<TClass>& ClassDefinitionT<TClass>::DefineNameAlias(const HashedString& name) noexcept
        {
            auto it = std::find
            (
                subject_.name_aliases_.begin(),
                subject_.name_aliases_.end(),
                name
            );

            if (it == subject_.name_aliases_.end())
            {
                subject_.name_aliases_.push_back(name);
            }

            return *this;
        }

        template <typename TClass>
        template <typename TBaseClass>
        ClassDefinitionT<TClass>& ClassDefinitionT<TClass>::DefineBaseClass() noexcept
        {
            static_assert(std::is_base_of_v<TBaseClass, TClass>, "TClass must derive from TBaseClass");
            static_assert(!std::is_same<TBaseClass, TClass>::value, "TClass cannot derive from itself");

            auto base_class = std::addressof(ClassOf<TBaseClass>());

            auto it = std::find
            (
                subject_.base_classes_.begin(),
                subject_.base_classes_.end(),
                base_class
            );

            if (it == subject_.base_classes_.end())
            {
                subject_.base_classes_.push_back(base_class);
            }

            return *this;
        }

        template <typename TClass>
        template <typename... TAccessors>
        PropertyDefinitionT<TAccessors...> ClassDefinitionT<TClass>::DefineProperty(const HashedString& property_name, TAccessors... accessors)
        {
            auto it = std::find_if
            (
                subject_.properties_.begin(),
                subject_.properties_.end(),
                [&property_name](const Property& property)
                {
                    return property.GetName() == property_name;
                }
            );

            if (it != subject_.properties_.end())
            {
                SYNTROPY_ERROR((ReflectionCtx), "A property named '", property_name, "' was already defined in the class '", subject_.default_name_, "'.");
            }

            subject_.properties_.emplace_back(property_name, accessors...);                             // Create a new property.

            return PropertyDefinitionT<TAccessors...>(subject_.properties_.back(), accessors...);       // Return a definition to allow property extensibility.
        }

        template <typename TClass>
        template <typename TInterface, typename TConcrete, typename... TArgs>
        ClassDefinitionT<TClass>& ClassDefinitionT<TClass>::AddInterface(TArgs&&... arguments)
        {
            if (subject_.interfaces_.AddInterface<TInterface, TConcrete>(std::forward<TArgs>(arguments)...) == nullptr)
            {
                SYNTROPY_ERROR((ReflectionCtx), "An interface '", typeid(TInterface).name(), "' was already added to the class '", subject_.default_name_, "'. The new interface has been ignored.");
            }

            return *this;
        }

        template <typename TClass>
        template <typename TFunctor>
        ClassDefinitionT<TClass>& ClassDefinitionT<TClass>::operator<<(TFunctor&& functor)
        {
            functor(*this);
            return *this;
        }

        /************************************************************************/
        /* GENERATE CLASS NAME                                                  */
        /************************************************************************/

        /// \brief Functor used to generate a template parameter list name.
        /// \author Raffaele D. Facendola - May 2017
        template <typename THead, typename... TRest>
        struct TemplateArgumentListT
        {
            void Append(std::ostream& stream) const
            {
                stream << TypeOf<THead>() << ", " << TemplateArgumentList<TRest...>;
            }
        };

        /// \brief Specialization for a single template argument.
        template <typename THead>
        struct TemplateArgumentListT<THead>
        {
            void Append(std::ostream& stream) const
            {
                stream << TypeOf<THead>();
            }
        };

        /// Utility object for TemplateArgumentListT.
        template <typename... TArguments>
        constexpr TemplateArgumentListT<TArguments...> TemplateArgumentList{};

        template <typename... TArguments>
        std::ostream& operator<<(std::ostream& stream, const TemplateArgumentListT<TArguments...>& argument_list)
        {
            argument_list.Append(stream);
            return stream;
        }

        template <typename... TArguments>
        std::string GenerateClassNameT<TArguments...>::operator()(const char* class_name) const
        {
            std::stringstream name;

            name << class_name << "<" << TemplateArgumentList<TArguments...> << ">";

            return name.str();
        }
    }
}