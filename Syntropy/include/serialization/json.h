/// \file json.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once


#include "type_traits.h"

#include "serialization/serialization.h"
#include "serialization/json/json_deserializer.h"

#include "reflection/any.h"

namespace syntropy 
{

    namespace serialization
    {

        /// \brief Property interface used to grant JSON deserialization capabilities.
        /// A property defining this interface can be directly read from a JSON object.
        /// \author Raffaele D. Facendola - September 2016
        class JSONDeserializable
        {

        public:

            /// \brief Helper tag used to resolve conflicts among copy/move constructor and explicit initialization via property accessors.
            struct property_tag {};

            /// \brief Create a new interface via direct accessors to the property.
            /// \tparam TAccessors type of the accessors.
            /// \param accessors Pointer to the actual property variable / getter-setter functions.
            template <typename... TAccessors>
            JSONDeserializable(property_tag, TAccessors&&... accessors);

            /// \brief Copy constructor.
            JSONDeserializable(const JSONDeserializable& other) noexcept;

            /// \brief Move constructor.
            JSONDeserializable(JSONDeserializable&& other) noexcept;

            /// \brief Default destructor.
            JSONDeserializable::~JSONDeserializable();

            /// \brief Assignment operator.
            JSONDeserializable& operator=(JSONDeserializable other) noexcept;

            /// \brief Deserialize the property of the specified instance from a JSON object.
            /// The method attempts to deserialize the property implementing this interface via the provided JSON object.
            /// If the provided instance doesn't support the property this method does nothing.
            /// \param instance Instance declaring the property.
            /// \param json JSON object to deserialize.
            template <typename TInstance, typename = std::enable_if_t<!std::is_same<std::decay<TInstance>::type, reflection::Any>::value>>
            bool operator()(TInstance&& instance, const nlohmann::json& json) const;

            bool operator()(reflection::Any& instance, const nlohmann::json& json) const;

        private:

            /// \brief Swaps the content of this property interface with the content of the provided one.
            void Swap(JSONDeserializable& other) noexcept;

            /// Type used to avoid a dynamic allocation of the concrete class of Content.
            /// 1 pointer for the single vtable, 2 pointers for both the setter and the getter.
            using storage_t = typename std::aligned_storage_t<3 * sizeof(void*), std::alignment_of<void*>::value>;

            /// \brief Interface to the actual implementation of the deserialization logic.
            /// This interface is used to erase the concrete type of the property.
            struct IContent
            {

                /// \brief Attempts to deserialize the property of the specified instance via the provided JSON object.
                /// The method does nothing if the provided instance doesn't support the property.
                /// \param object Object instance declaring the property.
                /// \param json JSON object to deserialize.
                virtual bool Deserialize(reflection::Any& object, const nlohmann::json& json) const = 0;

                /// \brief Clone the content to another instance.
                virtual void Clone(storage_t& storage) const noexcept = 0;

            };

            /// \brief Concrete class implementing the deserialization logic, according to the property type (field, getter or getter/setter pair).
            /// \see Template specialization below.
            template <typename... TAccessors>
            struct Content : IContent {};
            
            storage_t content_;                     ///< \brief Used to dispatch the deserialization logic.

        };

        /// \brief Class interface used to grant JSON construction capabilities.
        /// A class defining this interface can be directly constructed via JSON object.
        /// \author Raffaele D. Facendola - September 2016
        struct JSONConstructible
        {
            /// \brief Create a new interface.
            template <typename TClass>
            JSONConstructible(tag_t<TClass>);

            /// \brief Construct a new instance via JSON object.
            /// \param json JSON object the object will be constructed form.
            /// \return Returns an instance of an object constructed via a JSON object. If the object could not be constructed with the provided JSON object, returns an empty instance.
            reflection::Any operator()(const nlohmann::json& json) const;
        };
        
        /// \brief Functor object used to assign the interface JSONDeserializable to properties.
        /// \author Raffaele D. Facendola - September 2016
        struct JSONRead 
        {
 
            /// \brief Add a JSONDeserializable interface to the provided property.
            /// \param property Property to add the interface to.
            /// \param accessors Concrete accessors to the property (such as pointer to member variables, getters and getter/setter pairs)
            template <typename... TAccessors>
            void operator()(reflection::PropertyDefinitionT<TAccessors...>& property, TAccessors... accessors) const;

        };

        struct JSONConstruct
        {
            /// \brief Add a JSONConstructible interface to the provided class.
            /// \param class_definition Definition of the class the interface will be added to.
            template <typename TClass>
            void operator()(reflection::ClassDefinitionT<TClass>& class_definition) const;
        };

    }

}

namespace syntropy {

    namespace serialization 
    {

        /************************************************************************/
        /* JSON DESERIALIZABLE                                                  */
        /************************************************************************/

        template <typename... TAccessors>
        JSONDeserializable::JSONDeserializable(property_tag, TAccessors&&... accessors)
        {
            using content_t = Content<std::remove_reference_t<TAccessors>...>;

            static_assert(sizeof(storage_t) >= sizeof(content_t), "Storage size is not enough.");

            new (&content_) content_t(std::forward<TAccessors>(accessors)...);
        }

        template <typename TInstance, typename>
        bool JSONDeserializable::operator()(TInstance&& instance, const nlohmann::json& json) const
        {
            return reinterpret_cast<const IContent*>(&content_)->Deserialize(&instance, json);
        }

        inline bool JSONDeserializable::operator()(reflection::Any& instance, const nlohmann::json& json) const
        {
            return reinterpret_cast<const IContent*>(&content_)->Deserialize(instance, json);
        }

        /************************************************************************/
        /* JSON DESERIALIZABLE :: CONTENT                                       */
        /************************************************************************/

        /// \brief Template specialization of JSONDeserializable::Content for properties defined via their member variable pointer.
        /// \author Raffaele D. Facendola - September 2016.
        template <typename TClass, typename TField>
        struct JSONDeserializable::Content<TField TClass::*> : JSONDeserializable::IContent
        {

        public:

            Content(TField TClass::* field)
                : field_(field)
            {

            }

            virtual bool Deserialize(reflection::Any& object, const nlohmann::json& json) const override
            {
                auto concrete_instance = reflection::AnyCast<TClass*>(&object);

                if (concrete_instance)
                {
                    auto value = JSONDeserializer<TField>(json);

                    if (value)
                    {
                        (*concrete_instance)->*field_ = *std::move(value);
                        return true;
                    }
                }

                return false;
            }

            virtual void Clone(storage_t& storage) const noexcept override
            {
                using this_t = std::remove_reference_t<decltype(*this)>;

                static_assert(sizeof(storage_t) >= sizeof(this_t), "Storage size is not enough for an instance of this class.");

                new (&storage) this_t(field_);
            }

            TField TClass::* field_;                ///< \brief Member field.
        };

        /// \brief Template specialization of JSONDeserializable::Content for properties defined via a getter/setter member function pointer pair.
        /// The getter has the form - Property GetProperty() const;
        /// THe setter has the form - * SetProperty(Property);
        /// \author Raffaele D. Facendola - September 2016.
        template <typename TClass, typename TPropertyGet, typename TPropertySet, typename TAny>
        struct JSONDeserializable::Content<TPropertyGet(TClass::*)() const, TAny(TClass::*)(TPropertySet)> : JSONDeserializable::IContent
        {

            Content(TPropertyGet(TClass::*)() const, TAny(TClass::* setter)(TPropertySet))
                : setter_(setter) 
            {

            }

            virtual bool Deserialize(reflection::Any& object, const nlohmann::json& json) const override
            {
                using property_t = remove_reference_cv_t<TPropertySet>;

                auto concrete_instance = reflection::AnyCast<TClass*>(&object);

                if (concrete_instance)
                {
                    auto value = JSONDeserializer<property_t>(json);

                    if (value)
                    {
                        ((*concrete_instance)->*setter_)(*std::move(value));
                        return true;
                    }
                }

                return false;
            }

            virtual void Clone(storage_t& storage) const noexcept override
            {
                using this_t = std::remove_reference_t<decltype(*this)>;

                static_assert(sizeof(storage_t) >= sizeof(this_t), "Storage size is not enough for an instance of this class.");

                new (&storage) this_t(nullptr, setter_);
            }

            TAny(TClass::* setter_)(TPropertySet);          ///< \brief Setter method of the property.

        };

        /// \brief Template specialization of JSONDeserializable::Content for properties defined via a getter/setter member function pointer pair.
        /// The getter has the form - const Property& GetProperty() const;
        /// THe setter has the form - Property& GetProperty();
        /// \author Raffaele D. Facendola - September 2016.
        template <typename TClass, typename TPropertyGet, typename TPropertySet>
        struct JSONDeserializable::Content<const TPropertyGet&(TClass::*)() const, TPropertySet&(TClass::*)()> : JSONDeserializable::IContent
        {

            Content(const TPropertyGet&(TClass::*)() const, TPropertySet&(TClass::* setter)())
                : setter_(setter)
            {

            }

            virtual bool Deserialize(reflection::Any& object, const nlohmann::json& json) const override
            {
                auto concrete_instance = reflection::AnyCast<TClass*>(&object);

                if (concrete_instance)
                {
                    auto value = JSONDeserializer<TPropertySet>(json);

                    if (value)
                    {
                        ((*concrete_instance)->*setter_)() = *std::move(value);
                    }
                }

                return false;
            }

            virtual void Clone(storage_t& storage) const noexcept override
            {
                using this_t = std::remove_reference_t<decltype(*this)>;

                static_assert(sizeof(storage_t) >= sizeof(this_t), "Storage size is not enough for an instance of this class.");

                new (&storage) this_t(nullptr, setter_);
            }

            TPropertySet&(TClass::* setter_)();                   ///< \brief Setter method of the property.

        };

        /************************************************************************/
        /* JSON CONSTRUCTIBLE                                                   */
        /************************************************************************/

        template <typename TClass>
        JSONConstructible::JSONConstructible(tag_t<TClass>)
        {

        }

        /************************************************************************/
        /* JSON READ                                                            */
        /************************************************************************/

        template <typename... TAccessors>
        void JSONRead::operator()(reflection::PropertyDefinitionT<TAccessors...>& property, TAccessors... accessors) const
        {
            property.AddInterface<JSONDeserializable>
            (
                JSONDeserializable::property_tag(),
                accessors...
            );
        }

        /************************************************************************/
        /* JSON CONSTRUCT                                                       */
        /************************************************************************/

        template <typename TClass>
        void JSONConstruct::operator()(reflection::ClassDefinitionT<TClass>& class_definition) const
        {
            class_definition.AddInterface<JSONConstructible>(tag_t<TClass>{});
        }

    }

}
