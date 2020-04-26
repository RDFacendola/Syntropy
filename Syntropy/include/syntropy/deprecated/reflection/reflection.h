
/// \file reflection.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include "syntropy/containers/map.h"
#include <typeindex>

#include "syntropy/types/label.h"
#include "syntropy/containers/context.h"

#include "syntropy/reflection/class.h"

namespace syntropy::reflection
{
    /************************************************************************/
    /* REFLECTION                                                           */
    /************************************************************************/

    /// \brief Contains the list of all classes registered so far.
    /// \author Raffaele D. Facendola - 2016
    class Reflection
    {
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
        /// A default class name always takes precedence over aliases.
        /// \param class_name Name or alias of the class to get.
        /// \return Returns a pointer to the class whose name is the specified one, if any. Returns nullptr otherwise.
        const Class* GetClass(const Label& class_name) const noexcept;

        /// \brief Get a class instance by type index.
        /// This method can be used to retrieve the dynamic class of a polymorphic object.
        /// \param type_info Type index of the class to get.
        /// \return Returns a pointer to the class whose type index is the specified one, if any. Returns nullptr otherwise.
        const Class* GetClass(const std::type_index& type_index) const noexcept;

    private:

        /// \brief Private constructor to prevent instantiation and inheritance.
        Reflection();

        /// \brief Register a new class the the reflection system..
        /// \param class_t Class to register.
        void RegisterClass(const Class& class_t);

        Map<Label, const Class*> default_classes_;            ///< \brief Associates a default name to each registered class.

        Map<Label, const Class*> aliases_classes_;            ///< \brief Associates each name alias to each registered class.

        Map<std::type_index, const Class*>  typeindex_classes_;      ///< \brief Associates a type_index to each registered class.
    };

    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/

    /// \brief Get a class by name.
    /// \param class_name Name or alias of the class to get.
    /// \return Returns a pointer to the class whose name is the specified one, if any. Returns nullptr otherwise.
    const Class* GetClass(const Label& class_name) noexcept;

    /// \brief Get a class by type info.
    /// This method can be used to retrieve the dynamic class of a polymorphic object.
    /// \param type_info Type info of the class to get.
    /// \return Returns a pointer to the class whose type_index is the specified one, if any. Returns nullptr otherwise.
    const Class* GetClass(const std::type_info& type_info) noexcept;

    /// \brief Utility method used to get the dynamic class type of an object.
    /// \param object Object to get the dynamic class of.
    /// \return Returns a reference to the class describing TType.
    template <typename TType>
    const Class& ClassOf(const TType& object)
    {
        if constexpr(std::is_polymorphic_v<TType>)
        {
            if (auto object_class = GetClass(typeid(object)))
            {
                return *object_class;           // Return the dynamic type of object.
            }
        }
        else
        {
            SYNTROPY_UNUSED(object);
        }

        return ClassOf<TType&&>();              // The object is either not polymorphic or its dynamic type is not registered to the reflection system: returns the static class.
    }

    /************************************************************************/
    /* AUTO REGISTER CLASS                                                  */
    /************************************************************************/

    /// \brief Automatically register a class to syntropy reflection system.
    ///
    /// \usage (in my_class.cpp) AutoRegisterClass<MyClass> auto_class;
    /// \author Raffaele D. Facendola - February 2018
    template <typename TClass>
    struct AutoRegisterClass
    {
        AutoRegisterClass()
        {
            // Will trigger class registration.
            SYNTROPY_UNUSED(ClassOf<TClass>());
        }
    };
}