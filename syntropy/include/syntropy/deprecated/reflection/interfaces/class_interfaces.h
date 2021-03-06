
/// \file class_interfaces.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <optional>
#include <algorithm>
#include "syntropy/containers/map.h"

#include "syntropy/types/string.h"

#include "syntropy/type_traits.h"

#include "syntropy/reflection/any.h"

#include "syntropy/diagnostics/log.h"

namespace Syntropy::reflection
{
    template <typename TClass>
    class ClassT;

    /// \brief Class interface used to instantiate new objects via explicit constructor.
    /// \tparam TConstructorArguments Type of the arguments passed to the class constructor.
    /// \author Raffaele D. Facendola - April 2017
    template <typename... TConstructorArguments>
    class Constructible
    {
    public:

        /// \brief Create a new interface.
        template <typename TClass>
        Constructible(ClassT<TClass>&)
            : instancer_(&Instantiate<TClass>)
        {
            static_assert(std::is_constructible<TClass, TConstructorArguments...>::value, "TClass is not constructible with arguments TConstructorArguments.");
        }

        /// \brief Instantiate a new object.
        /// \param arguments Arguments to pass to the constructor.
        template <typename... TArguments>
        Any operator()(TArguments&&... arguments) const
        {
            return (*instancer_)(Forward<TArguments>(arguments)...);
        }

    private:

        /// \brief Instantiate a new object.
        /// \param arguments Arguments to pass to the constructor.
        template <typename TClass>
        static Any Instantiate(TConstructorArguments... arguments)
        {
            // TODO: if the caller doesn't take owneship of this object it will leak. Use std::unique_ptr<T> instead...
            return new TClass(Forward<TConstructorArguments>(arguments)...);
        }

        Any(*instancer_)(TConstructorArguments...);                     /// \brief Functor used to instantiate the class.

    };

    /// \brief Class interface used to provide enumeration capabilities.
    /// \author Raffaele D. Facendola - June 2017
    class Enumeration
    {
    public:

        /// \brief Create a new interface.
        /// \param values Enumeration values pairs: the first element is the enumeration value name, the second element is the actual enumeration value.
        template <typename TEnum>
        Enumeration(const Map<Label, TEnum>& values)
        {
            std::transform(values.begin(), values.end(), std::inserter(values_, values_.end()), [](auto&& value)
            {
                return std::make_pair(value.first, value.second);
            });
        }

        /// \brief Get an enumeration value by name.
        /// \param value_name Name of the enumeration value to get.
        /// \return Returns the enumeration value associated to value_name. If no such enumeration value could be found returns std::nullopt.
        template <typename TEnum>
        std::optional<TEnum> GetValueByName(const Label& value_name) const
        {
            auto value = GetValueByName(value_name);

            if (auto enum_value = AnyCast<TEnum>(&value))
            {
                return *enum_value;
            }

            return std::nullopt;
        }

        /// \brief Get an enumeration value name by name.
        /// \param value Value of the enumeration value name to get.
        /// \return Returns the enumeration value name associated to value. If no such name could be found returns std::nullopt.
        template <typename TEnum>
        std::optional<Label> GetNameByValue(const TEnum& value) const
        {
            auto it = std::find_if(values_.begin(), values_.end(), [&value](auto&& enum_value_pair)
            {
                auto enum_value = AnyCast<TEnum>(&(enum_value_pair.second));

				return enum_value && *enum_value == value;
            });

            return (it != values_.end()) ? std::make_optional(it->first) : std::nullopt;
        }

    private:

        /// \brief Get an enumeration value by name.
        /// \param value_name Name of the enumeration value to get.
        /// \return Returns the enumeration value associated to value_name. If no such enumeration value could be found returns an empty object.
        Any GetValueByName(const Label& value_name) const;

        Map<Label, Any> values_;      ///< \brief Registered enumeration values.
    };

    /// \brief Functor object used to give enumeration capabilities to classes.
    /// \author Raffaele D. Facendola - June 2017
    template <typename TEnum>
    struct EnumerationClass
    {
        /// \brief Create a new functor.
        /// \param values Enumeration values pairs: the first element is the enumeration value name, the second element is the actual enumeration value.
        EnumerationClass(const Map<Label, TEnum>& values)
            : values_(values)
        {

        }

        /// \brief Add a Enumeration interface to the provided class.
        /// \param class_definition Definition of the class the interface will be added to.
        void operator()(reflection::ClassT<TEnum>& class_t) const
        {
            class_t.AddInterface<Enumeration>(values_);
        }

    private:

        const Map<Label, TEnum>& values_;         ///< \brief Enumeration values to register.
    };
}
