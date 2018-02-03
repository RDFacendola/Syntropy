
/// \file class.h
/// \brief This header is part of the syntropy reflection system. It contains classes used to handle class definition.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <vector>
#include <algorithm>
#include <typeindex>

#include "type_traits.h"

#include "patterns/utility.h"

#include "containers/hashed_string.h"
#include "containers/interface_container.h"

#include "reflection/type.h"
#include "reflection/property.h"
#include "reflection/interfaces/class_interfaces.h"

#include "diagnostics/log.h"

namespace syntropy::reflection
{
    /************************************************************************/
    /* CLASS DECLARATION                                                    */
    /************************************************************************/

    /// \brief Represents a class declaration.
    /// IMPORTANT: Provide a specialization for each class requiring reflection.
    template <typename TClass>
    struct ClassDeclarationT
    {
        // Name of the class.
        // static constexpr const char* name_{"MyClass"};

        // Fill class definition with properties, methods, base classes, etc.
        // void operator()(TDefinition& definition) const;
    };

    /// \brief Helper variable for ClassDeclarationT<TClass>
    template <typename TClass>
    inline constexpr ClassDeclarationT<TClass> ClassDeclaration{};

    /************************************************************************/
    /* CLASS NAME T                                                         */
    /************************************************************************/

    /// \brief Provides a member function used to get TClass class name.
    template <typename TClass>
    struct ClassNameT
    {
        static auto GetName()
        {
            return ClassDeclarationT<TClass>::name_;
        }
    };

    /// \brief Partial specialization for class templates (recursive).
    template <template <typename...> typename TClass, typename THead, typename... TRest>
    struct ClassNameT<TClass<THead, TRest...>>
    {
        static auto GetName()
        {
            std::stringstream name;

            name << ClassDeclarationT<TClass<THead, TRest...>>::name_ << "<" << TypeOf<THead>();

            ((name << ", " << TypeOf<TRest>()), ...);

            name << ">";

            return name.str();
        }
    };

    /************************************************************************/
    /* CLASS                                                                */
    /************************************************************************/

    /// \brief Describes a class.
    /// A class can be used to access fields, properties and methods.
    /// \remarks This class is a singleton.
    /// \author Raffaele D. Facendola - 2016
    class Class
    {
    public:

        /// \brief Get the class associated to TClass.
        /// \return Returns a reference to the singleton describing the class TClass.
        template <typename TClass>
        static const Class& GetClass()
        {
            static Class instance(tag<TClass>);

            return instance;
        }

        /// \brief No copy constructor.
        Class(const Class&) = delete;

        /// \brief No assignment operator.
        Class& operator=(const Class&) = delete;

        /// \brief Virtual destructor.
        virtual ~Class() = default;

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

        /// \brief Get the type index of the underlying class.
        /// \return Returns the type index of the underlying class.
        const std::type_index& GetTypeIndex() const noexcept;

        /// \brief Check whether this class is abstract or not.
        /// \return Returns true if the class is abstract, returns false otherwise.
        bool IsAbstract() const noexcept;

        /// \brief Query the class for an interface of type TInterface.
        /// Only interfaces defined in this class are checked.
        /// \return If an interface of type TInterface was added during class declaration, returns a pointer to that interface, otherwise returns nullptr.
        /// \remarks This method doesn't account for polymorphism. If a class of type Foo derived from Bar is added to the class, GetInterface<Bar>() will return nullptr even if a conversion exists.
        template <typename TInterface>
        const TInterface* GetInterface() const
        {
            return interfaces_.GetInterface<TInterface>();
        }

        /// \brief Define a name alias for this class.
        /// If the provided alias already exists, this method does nothing.
        /// \param name New name alias.
        void AddNameAlias(HashedString name_alias);

        /// \brief Define a base class for this class.
        /// If the provided base class already exists, this method does nothing.
        /// If the provided base class is NOT an actual base class for the class described by this class the behavior is undefined.
        /// \param base_class Base class.
        void AddBaseClass(const Class& base_class);

        /// \brief Add a new property to this class.
        /// If the provided property doesn't belong to this class the behavior is undefined.
        /// \param name Unique name of the class property.
        /// \param accessors Member field or methods used to access the property being defined.
        /// \return Returns the property definition.
        template <typename... TAccessors>
        auto AddProperty(const HashedString& property_name, TAccessors... accessors)
        {
            if (GetProperty(property_name))
            {
                SYNTROPY_CRITICAL((ReflectionCtx), "A property named '", property_name, "' was already defined in the class '", default_name_, "'.");
            }

            auto& property = properties_.emplace_back(property_name, accessors...);

            return PropertyDefinitionT<TAccessors...>(property, accessors...);              // Returns a definition to allow property extensibility.
        }

        /// \brief Add a new interface to this class.
        /// The method creates an instance of TConcrete using TArgs as construction parameters. Only one interface of type TInterface can be added per class.
        /// TConcrete must be equal to or derive from TInterface.
        /// \param arguments Arguments to pass to the constructor of TInterface.
        template <typename TInterface, typename TConcrete = TInterface, typename... TArgs>
        void AddInterface(TArgs&&... arguments)
        {
            if (interfaces_.AddInterface<TInterface, TConcrete>(std::forward<TArgs>(arguments)...) == nullptr)
            {
                SYNTROPY_ERROR((ReflectionCtx), "An interface '", typeid(TInterface).name(), "' was already part of the class '", default_name_, "'. The new interface has been ignored.");
            }
        }

    private:

        /// \brief Create a new class.
        template <typename TClass>
        Class(tag_t<TClass>)
            : default_name_(ClassNameT<TClass>::GetName())              // #TODO Add support to alias derived from namespaces!
            , type_index_(typeid(TClass))
            , is_abstract_(std::is_abstract<TClass>::value)
        {
            static_assert(is_class_name_v<TClass>, "TClass must be a plain class name (without pointers, references, extents and/or qualifiers)");

            // Common interfaces.

            if constexpr(std::is_default_constructible_v<TClass>)
            {
                AddInterface<Constructible<>>(tag<TClass>);
            }

            // Declare class members, base classes, etc.

            if constexpr(std::is_invocable_v<ClassDeclarationT<TClass>, ClassDefinitionT<TClass>&>)
            {
                ClassDefinitionT<TClass> definition(*this);

                ClassDeclaration<TClass>(definition);
            }

            // Register the class to the reflection system.

            Reflection::GetInstance().Register(*this);
        }

        /// \brief Create a new class for the void type.
        Class(tag_t<void>);

        HashedString default_name_;                     ///< \brief Default class name.

        std::vector<HashedString> name_aliases_;        ///< \brief Class name aliases.

        std::vector<const Class*> base_classes_;        ///< \brief List of all base classes.

        std::vector<Property> properties_;              ///< \brief Class properties.

        InterfaceContainer interfaces_;                 ///< \brief Interfaces assigned to this class.

        std::type_index type_index_;                    ///< \brief Type index the type this object refers to.

        bool is_abstract_;                              ///< \brief Whether the class is abstract.
    };

    /// \brief Test two classes for equality.
    /// This method check if two classes are exactly the same.
    /// \brief Returns true if lhs class is the same as rhs class, returns false otherwise.
    /// \see See Class:IsA(...) if polymorphic comparison is needed.
    bool operator==(const Class& lhs, const Class& rhs) noexcept;

    /// \brief Test two classes for inequality.
    /// This method check if two classes are not the same.
    /// \brief Returns true if lhs class is not the same as rhs class, returns false otherwise.
    bool operator!=(const Class& lhs, const Class& rhs) noexcept;

    /// \brief Utility method used to get a class by type.
    /// \return Returns a reference to the class describing TType.
    template <typename TType>
    const Class& ClassOf()
    {
        return Class::GetClass<class_name_t<TType>>();
    }

    /// \brief Stream insertion for Class.
    std::ostream& operator<<(std::ostream& out, const Class& class_instance);

    /************************************************************************/
    /* CLASS DEFINITION                                                     */
    /************************************************************************/

    /// \brief Concrete class definition.
    /// This class is used to safely define class names, properties, methods, etc.
    /// \author Raffaele D. Facendola - 2016
    template <typename TClass>
    class ClassDefinitionT
    {
    public:

        /// \brief Apply a functor to a class definition.
        /// \param class_definition Target class definition.
        /// \param functor Functor to apply.
        /// \return Returns the class definition.
        template <typename TFunctor>
        friend ClassDefinitionT& operator<<(ClassDefinitionT& class_definition, TFunctor&& functor)
        {
            functor(class_definition);
            return class_definition;
        }

        /// \brief Create a new class definition.
        /// \param subject Class this definition refers to.
        explicit ClassDefinitionT(Class& subject)
            : class_(subject)
        {

        }

        /// \brief Define a name alias for the class.
        /// If the name was already defined this method does nothing.
        /// \param name Name alias.
        void DefineNameAlias(const HashedString& name_alias) noexcept
        {
            class_.AddNameAlias(name_alias);
        }

        /// \brief Define a base class.
        /// If the base class was already defined this method does nothing.
        /// \tparam TBaseClass Type of the base class.
        template <typename TBaseClass>
        void DefineBaseClass() noexcept
        {
            static_assert(std::is_base_of_v<TBaseClass, TClass>, "TClass must derive from TBaseClass.");
            static_assert(!std::is_same<TBaseClass, TClass>::value, "TClass cannot derive from itself.");

            class_.AddBaseClass(ClassOf<TBaseClass>());
        }

        /// \brief Define a class property.
        /// \param name Unique name of the class property.
        /// \param accessors Member field or methods used to access the property being defined.
        /// \return Returns the property definition.
        template <typename... TAccessors>
        auto DefineProperty(const HashedString& property_name, TAccessors... accessors)
        {
            // #TODO Check if the provided accessors refer to TClass.

            return class_.AddProperty(property_name, std::forward<TAccessors>(accessors)...);
        }

        /// \brief Add a new interface to the class.
        /// The method creates an instance of TConcrete using TArgs as construction parameters. Only one interface of type TInterface can be added per class.
        /// TConcrete must be equal to or derive from TInterface.
        /// \param arguments Arguments to pass to the constructor of TInterface.
        template <typename TInterface, typename TConcrete = TInterface, typename... TArgs>
        void AddInterface(TArgs&&... arguments)
        {
            class_.AddInterface<TInterface, TConcrete>(std::forward<TArgs>(arguments)...);
        }

    private:

        Class& class_;            ///< \brief Class this definition refers to.

    };
}

