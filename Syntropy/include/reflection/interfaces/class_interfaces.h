
/// \file class_interfaces.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <optional>
#include <algorithm>
#include <unordered_map>

#include "type_traits.h"

#include "reflection/any.h"

#include "diagnostics/log.h"

namespace syntropy
{
    namespace reflection
    {

        template <typename TClass>
        class ClassDefinitionT;

        /// \brief Class interface used to instantiate new objects via explicit constructor.
        /// \tparam TConstructorArguments Type of the arguments passed to the class constructor.
        /// \author Raffaele D. Facendola - April 2017
        template <typename... TConstructorArguments>
        class Constructible
        {
        public:

            /// \brief Create a new interface.
            template <typename TClass>
            Constructible(tag_t<TClass>);

            /// \brief Instantiate a new object.
            /// \param arguments Arguments to pass to the constructor.
            template <typename... TArguments>
            Any operator()(TArguments&&... arguments) const;

        private:

            /// \brief Instantiate a new object.
            /// \param arguments Arguments to pass to the constructor.
            template <typename TClass>
            static Any Instantiate(TConstructorArguments... arguments);

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
            Enumeration(const std::unordered_map<HashedString, TEnum>& values);

            /// \brief Get an enumeration value by name.
            /// \param value_name Name of the enumeration value to get.
            /// \return Returns the enumeration value associated to value_name. If no such enumeration value could be found returns std::nullopt.
            template <typename TEnum>
            std::optional<TEnum> GetValueByName(const HashedString& value_name) const;

            /// \brief Get an enumeration value name by name.
            /// \param value Value of the enumeration value name to get.
            /// \return Returns the enumeration value name associated to value. If no such name could be found returns std::nullopt.
            template <typename TEnum>
            std::optional<HashedString> GetNameByValue(const TEnum& value) const;

        private:

            /// \brief Get an enumeration value by name.
            /// \param value_name Name of the enumeration value to get.
            /// \return Returns the enumeration value associated to value_name. If no such enumeration value could be found returns an empty object.
            Any GetValueByName(const HashedString& value_name) const;

            std::unordered_map<HashedString, Any> values_;      ///< \brief Registered enumeration values.
        };

        /// \brief Functor object used to give enumeration capabilities to classes.
        /// \author Raffaele D. Facendola - June 2017
        template <typename TEnum>
        struct EnumerationClass
        {
            /// \brief Create a new functor.
            /// \param values Enumeration values pairs: the first element is the enumeration value name, the second element is the actual enumeration value.
            EnumerationClass(const std::unordered_map<HashedString, TEnum>& values);

            /// \brief Add a Enumeration interface to the provided class.
            /// \param class_definition Definition of the class the interface will be added to.
            void operator()(ClassDefinitionT<TEnum>& class_definition) const;

        private:

            const std::unordered_map<HashedString, TEnum>& values_;         ///< \brief Enumeration values to register.
        };

    }
}

// Implementation

namespace syntropy
{
    namespace reflection
    {

        /************************************************************************/
        /* CONSTRUCTIBLE                                                        */
        /************************************************************************/

        template <typename... TConstructorArguments>
        template <typename TClass>
        Constructible<TConstructorArguments...>::Constructible(tag_t<TClass>)
            : instancer_(&Instantiate<TClass>)
        {
            static_assert(std::is_constructible<TClass, TConstructorArguments...>::value, "TClass is not constructible with arguments TConstructorArguments.");
        }

        template <typename... TConstructorArguments>
        template <typename... TArguments>
        Any Constructible<TConstructorArguments...>::operator()(TArguments&&... arguments) const
        {
            return (*instancer_)(std::forward<TArguments>(arguments)...);
        }

         template <typename... TConstructorArguments>
         template <typename TClass>
        Any Constructible<TConstructorArguments...>::Instantiate(TConstructorArguments... arguments)
         {
             // TODO: if the caller doesn't take owneship of this object it will leak. Use std::unique_ptr<T> instead...
             return new TClass(std::forward<TConstructorArguments>(arguments)...);
         }

        /************************************************************************/
        /* ENUMERATION                                                          */
        /************************************************************************/

        template <typename TEnum>
        Enumeration::Enumeration(const std::unordered_map<HashedString, TEnum>& values)
        {
            std::transform(values.begin(), values.end(), std::inserter(values_, values_.end()), [](auto&& value)
            {
                return std::make_pair(value.first, value.second);
            });
        }

        template <typename TEnum>
        std::optional<TEnum> Enumeration::GetValueByName(const HashedString& value_name) const
        {
            auto value = GetValueByName(value_name);

            auto enum_value = AnyCast<TEnum>(&value);

            if (enum_value)
            {
                return *enum_value;
            }

            return std::nullopt;
        }

        template <typename TEnum>
        std::optional<HashedString> Enumeration::GetNameByValue(const TEnum& value) const
        {
            auto it = std::find_if(values_.begin(), values_.end(), [&value](auto&& value)
            {
                auto enum_value = AnyCast<TEnum>(&(value.second));

                return enum_value && *enum_value == value;
            });

            if (it != values_.end())
            {
                return it->first;
            }

            return std::nullopt;
        }

        /************************************************************************/
        /* ENUMERATION CLASS                                                    */
        /************************************************************************/

        template <typename TEnum>
        EnumerationClass<TEnum>::EnumerationClass(const std::unordered_map<HashedString, TEnum>& values)
            : values_(values)
        {

        }

        template <typename TEnum>
        void EnumerationClass<TEnum>::operator()(reflection::ClassDefinitionT<TEnum>& class_definition) const
        {
            class_definition.AddInterface<Enumeration>(values_);
        }

    }
}
