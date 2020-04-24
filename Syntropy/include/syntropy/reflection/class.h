
/// \file class.h
/// \brief This header is part of the syntropy reflection system. It contains classes used to handle class definition.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <vector>
#include <algorithm>
#include <typeindex>

#include "syntropy/type_traits.h"
#include "syntropy/contexts.h"

#include "syntropy/patterns/utility.h"

#include "syntropy/types/label.h"

#include "syntropy/patterns/multi_interface.h"

#include "syntropy/reflection/type.h"
#include "syntropy/reflection/property.h"
#include "syntropy/reflection/interfaces/class_interfaces.h"

#include "syntropy/diagnostics/log.h"

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
        // void operator()(ClassT<TClass>& definition) const;
    };

    /// \brief Helper variable for ClassDeclarationT<TClass>
    template <typename TClass>
    inline constexpr ClassDeclarationT<TClass> ClassDeclaration{};

    /************************************************************************/
    /* CLASS                                                                */
    /************************************************************************/

    /// \brief Describes a class.
    /// A class can be used to access fields, properties and methods.
    /// \author Raffaele D. Facendola - 2016
    class Class : public MultiInterfaceMixin<>
    {
    public:

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
        const Label& GetDefaultName() const noexcept;

        /// \brief Get all the class name aliases except for the default class name.
        /// Certain types have different name aliases (like signed short int, short, short int which refer to the same type).
        /// \brief Returns a list containing the class names.
        const std::vector<Label>& GetNameAliases() const noexcept;

        /// \brief Get the list of the base classes of this class.
        /// \return Returns the list of classes that are extended by this class.
        const std::vector<const Class*>& GetBaseClasses() const noexcept;

        /// \brief Get a class property by name.
        /// Only properties defined in this class are checked.
        /// \return Returns a pointer to the requested property. If no such property could be found returns nullptr.
        const Property* GetProperty(const Label& property_name) const noexcept;

        /// \brief Get the list of properties supported by this class.
        /// \return Returns the list of properties supported by this class.
        const std::vector<Property>& GetProperties() const noexcept;

        /// \brief Get the type index of the underlying class.
        /// \return Returns the type index of the underlying class.
        const std::type_index& GetTypeIndex() const noexcept;

        /// \brief Check whether this class is abstract or not.
        /// \return Returns true if the class is abstract, returns false otherwise.
        bool IsAbstract() const noexcept;

    protected:

        // Create a new class from a type.
        template <typename TClass>
        Class(tag_t<TClass>)
            : default_name_(TypeNameT<TClass>::GetName())      // #TODO Add support to alias derived from namespaces!
            , type_index_(typeid(TClass))
            , is_abstract_(std::is_abstract<TClass>::value)
        {
            RegisterClass();
        }

        // Register this class to the reflection system.
        void RegisterClass();

        Label default_name_;                     ///< \brief Default class name.

        std::vector<Label> name_aliases_;        ///< \brief Class name aliases.

        std::vector<const Class*> base_classes_;        ///< \brief List of all base classes.

        std::vector<Property> properties_;              ///< \brief Class properties.

        std::type_index type_index_;                    ///< \brief Type index the type this object refers to.

        bool is_abstract_;                              ///< \brief Whether the class is abstract.
    };

    /// \brief Describes a concrete class.
    /// \author Raffaele D. Facendola - 2018
    /// \remarks This class is a singleton.
    template <typename TClass>
    class ClassT : public Class
    {
        static_assert(is_class_name_v<TClass>, "TClass must be a plain class name (without pointers, references, extents and/or qualifiers)");

    public:

        /// \brief Get the class singleton.
        /// \return Returns a reference to the singleton describing TClass.
        static const Class& GetClass()
        {
            static ClassT<TClass> instance;

            return instance;
        }

        /// \brief No copy constructor.
        ClassT(const ClassT&) = delete;

        /// \brief No assignment operator.
        ClassT& operator=(const ClassT&) = delete;

        /// \brief Define a name alias for this class.
        /// If the provided alias already exists, this method does nothing.
        /// \param name New name alias.
        void AddNameAlias(Label name_alias)
        {
            if (std::find(name_aliases_.begin(), name_aliases_.end(), name_alias) == name_aliases_.end())
            {
                name_aliases_.push_back(name_alias);
            }
        }

        /// \brief Define a base class for this class.
        /// If the provided base class already exists, this method does nothing.
        /// If the provided base class is NOT an actual base class for the class described by this class the behavior is undefined.
        template <typename TBaseClass>
        void AddBaseClass()
        {
            static_assert(std::is_base_of_v<TBaseClass, TClass>, "TBaseClass is not a base class for this class.");
            static_assert(!std::is_same<TBaseClass, TClass>::value, "A class may not derive from itself.");

            auto& base_class = ClassOf<TBaseClass>();

            if (std::find(base_classes_.begin(), base_classes_.end(), &base_class) == base_classes_.end())
            {
                base_classes_.push_back(&base_class);
            }
        }

        /// \brief Add a new property to this class.
        /// If the provided property doesn't belong to this class the behavior is undefined.
        /// \param name Unique name of the class property.
        /// \param accessors Member field or methods used to access the property being defined.
        /// \return Returns the property definition.
        template <typename... TAccessors>
        auto AddProperty(const Label& property_name, TAccessors... accessors)
        {
            // #TODO Check if TAccessors refer to TClass.

            if (GetProperty(property_name))
            {
                SYNTROPY_CRITICAL((ReflectionCtx), "A property named '", property_name, "' was already defined in the class '", default_name_, "'.");
            }

            auto& property = properties_.emplace_back(property_name, accessors...);

            return PropertyDefinitionT<TAccessors...>(property, accessors...);              // Returns a definition to allow property extensibility.
        }

        /// \brief Apply a functor to this class.
        /// \param class_t Target concrete class.
        /// \param functor Functor to apply.
        /// \return Returns the class definition.
        template <typename TFunctor>
        friend auto& operator<<(ClassT& class_t, TFunctor&& functor)
        {
            functor(class_t);
            return class_t;
        }

    private:

        // Create a new concrete class.
        ClassT()
            : Class(tag<TClass>)
        {
            // Add common interfaces for this class.

            if constexpr(std::is_default_constructible_v<TClass>)
            {
                AddInterface<Constructible<>>(*this);
            }

            // Fill the concrete class with actual members, base classes, etc.

            if constexpr(std::is_invocable_v<ClassDeclarationT<TClass>, ClassT<TClass>&>)
            {
                ClassDeclaration<TClass>(*this);
            }
        }
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

    /// \brief Get a class by type.
    /// \return Returns a reference to the class describing TType.
    template <typename TType>
    const Class& ClassOf()
    {
        return ClassT<class_name_t<TType>>::GetClass();
    }

    /// \brief Stream insertion for Class.
    std::ostream& operator<<(std::ostream& out, const Class& class_t);
}

