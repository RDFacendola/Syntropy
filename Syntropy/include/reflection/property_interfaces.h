
/// \file property_interfaces.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2017

#pragma once

#include <functional>

#include "type_traits.h"

#include "reflection/any.h"
#include "reflection/property.h"

namespace syntropy
{
    namespace reflection
    {

        /// \brief Property interface used to read properties.
        /// \author Raffaele D. Facendola - April 2017
        class Readable
        {
        public:

            /// \brief Create a new interface by a member field.
            template <typename TClass, typename TProperty>
            Readable(TProperty(TClass::* field));

            /// \brief Create a new interface by const getter method.
            template <typename TClass, typename TProperty>
            Readable(TProperty(TClass::* getter)() const);

            /// \brief Read the property value.
            /// \param instance Object to write the property of. Expects pointer to the actual object instance.
            /// \return Returns an object containing a copy of the property value.
            Any operator()(const Any& instance) const;

            /// \brief Read the property value.
            /// This overload only participates in overload resolution if TInstance is not reflection::Any.
            /// \param instance Object to read the property of.
            /// \return Returns an object containing a copy of the property value.
            template <typename TInstance, typename = std::enable_if_t<!std::is_same_v<std::decay_t<TInstance>, reflection::Any>>>
            Any operator()(const TInstance& instance) const;

            /// \brief Read the property value.
            /// This overload only participates in overload resolution if TInstance is not reflection::Any.
            /// This method is used only if the target object is a rvalue reference to avoid unnecessary copies.
            /// \param instance Object to read the property of.
            /// \return Returns an object containing the moved property value.
            template <typename TInstance, typename = std::enable_if_t<!std::is_lvalue_reference<TInstance>::value && !std::is_same_v<std::decay_t<TInstance>, reflection::Any>>>
            Any operator()(TInstance&& instance) const;

        private:

            std::function<Any(const Any&)> reader_;     ///< \brief Functor used to copy-construct the returned property value.

            std::function<Any(Any&&)> mover_;           ///< \brief Functor used to move-construct the returned property value.

        };

        /// \brief Property interface used to write properties.
        /// \author Raffaele D. Facendola - April 2017
        class Writeable
        {
        public:

            /// \brief Create a new interface from a member field.
            /// The field must be copy-assignable.
            template <typename TClass, typename TProperty, typename = std::enable_if_t<std::is_member_object_pointer<TProperty(TClass::*)>::value>>
            Writeable(TProperty(TClass::* field));

            /// \brief Create a new interface from a setter method.
            template <typename TClass, typename TProperty>
            Writeable(void(TClass::* setter)(TProperty));

            /// \brief Create a new interface from a non-const accessor.
            template <typename TClass, typename TProperty>
            Writeable(TProperty&(TClass::* setter)());

            /// \brief Write the property value.
            /// \param instance Object to write the property of. Expects pointer to the actual object instance.
            /// \param value Value to write. Must match the property value.
            template <typename TValue>
            void operator()(const Any& instance, const TValue& value) const;

            /// \brief Write the property value.
            /// This method is used only if the value to write is a rvalue reference to avoid unnecessary copies.
            /// \param instance Object to write the property of. Expects pointer to the actual object instance.
            /// \param value Value to write. Must match the property value.
            template <typename TValue, typename = std::enable_if_t<!std::is_lvalue_reference<TValue>::value>>
            void operator()(const Any& instance, TValue&& value) const;

            /// \brief Write the property value.
            /// This overload only participates in overload resolution if TInstance is not reflection::Any.
            /// \param instance Object to write the property of. Expects pointer to the actual object instance.
            /// \param value Value to write. Must match the property value.
            template <typename TInstance, typename TValue, typename = std::enable_if_t<!std::is_same_v<std::decay_t<TInstance>, reflection::Any>>>
            void operator()(TInstance& instance, TValue&& value) const;

        private:

            std::function<void(const Any&, const Any&)> writer_;        ///< \brief Functor used to copy-assign\copy-construct the property value.

            std::function<void(const Any&, Any&&)> mover_;              ///< \brief Functor used to move-assign\move-construct the property value.

        };

    }
}

// Implementation

namespace syntropy
{
    namespace reflection
    {

        /************************************************************************/
        /* READABLE                                                             */
        /************************************************************************/

        template <typename TClass, typename TProperty>
        Readable::Readable(TProperty(TClass::* field))
        {
            static_assert(std::is_copy_constructible_v<std::remove_cv_t<TProperty>>, "std::remove_cv_t<TProperty> must be copy-constructible.");

            reader_ = [field](const Any& instance)
            {
                return AnyCast<const TClass*>(instance)->*field;            // Copy-construct the returned value.
            };

            mover_ = [field](Any&& instance)
            {
                return std::move(AnyCast<TClass*>(instance)->*field);       // Move-construct the returned value.
            };
        }

        template <typename TClass, typename TProperty>
        Readable::Readable(TProperty(TClass::* getter)() const)
        {
            static_assert(std::is_copy_constructible_v<remove_reference_cv_t<TProperty>>, "remove_reference_cv_t<TProperty> must be copy-constructible.");

            reader_ = [getter](const Any& instance)
            {
                return (AnyCast<const TClass*>(instance)->*getter)();
            };

            mover_ = [getter](Any&& instance)
            {
                return (AnyCast<const TClass*>(instance)->*getter)();
            };
        }

        inline Any Readable::operator()(const Any& instance) const
        {
            return reader_(instance);
        }

        template <typename TInstance, typename>
        Any Readable::operator()(const TInstance& instance) const
        {
            return reader_(&instance);
        }

        template <typename TInstance, typename>
        Any Readable::operator()(TInstance&& instance) const
        {
            return mover_(&instance);
        }

        /************************************************************************/
        /* WRITEABLE                                                            */
        /************************************************************************/

        template <typename TClass, typename TProperty, typename>
        Writeable::Writeable(TProperty(TClass::* field))
        {
            static_assert(std::is_copy_assignable_v<TProperty>, "TProperty must be copy-assignable");

            writer_ = [field](const Any& instance, const Any& value)
            {
                AnyCast<TClass*>(instance)->*field = AnyCast<const TProperty&>(value);              // Copy-assignment.
            };

            mover_ = [field](const Any& instance, Any&& value)
            {
                AnyCast<TClass*>(instance)->*field = AnyCast<TProperty>(std::move(value));          // Move-assignment.
            };
        }

        template <typename TClass, typename TProperty>
        Writeable::Writeable(void(TClass::* setter)(TProperty))
        {
            static_assert(std::is_copy_constructible_v<remove_reference_cv_t<TProperty>>, "remove_reference_cv_t<TProperty> must be copy-constructible");

            writer_ = [setter](const Any& instance, const Any& value)
            {
                (AnyCast<TClass*>(instance)->*setter)(AnyCast<const TProperty&>(value));            // Copy-construction.
            };

            mover_ = [setter](const Any& instance, Any&& value)
            {
                (AnyCast<TClass*>(instance)->*setter)(AnyCast<TProperty>(std::move(value)));        // Move-construction.
            };
        }

        template <typename TClass, typename TProperty>
        Writeable::Writeable(TProperty&(TClass::* setter)())
        {
            static_assert(std::is_copy_assignable_v<TProperty>, "TProperty must be copy-assignable");

            writer_ = [setter](const Any& instance, const Any& value)
            {
                (AnyCast<TClass*>(instance)->*setter)() = AnyCast<const TProperty&>(value);         // Copy-assignment.
            };

            mover_ = [setter](const Any& instance, Any&& value)
            {
                (AnyCast<TClass*>(instance)->*setter)() = AnyCast<TProperty>(std::move(value));     // Move-assignment.
            };
        }

        template <typename TValue>
        void Writeable::operator()(const Any& instance, const TValue& value) const
        {
            writer_(instance, value);
        }

        template <typename TValue, typename>
        void Writeable::operator()(const Any& instance, TValue&& value) const
        {
            mover_(instance, std::forward<TValue>(value));
        }

        template <typename TInstance, typename TValue, typename>
        void Writeable::operator()(TInstance& instance, TValue&& value) const
        {
            (*this)(std::addressof(instance), std::forward<TValue>(value));
        }

    }
}

