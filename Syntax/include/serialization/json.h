/// \file json.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once


#include <type_traits>

#include "serialization/json/json_deserializer.h"

namespace syntropy {

    namespace syntax {

        /// \brief Property interface used to grant JSON deserialization capabilities.
        /// A property defining this interface can be directly read from a JSON object.
        /// \author Raffaele D. Facendola - September 2016
        class JSONDeserializable{

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
            template <typename TInstance>
            bool operator()(TInstance&& instance, const nlohmann::json& json) const;

        private:

            /// \brief Swaps the content of this property interface with the content of the provided one.
            void Swap(JSONDeserializable& other) noexcept;

            /// Type used to avoid a dynamic allocation of the concrete class of Content.
            /// 1 pointer for the single vtable, 2 pointers for both the setter and the getter.
            using storage_t = typename std::aligned_storage_t<3 * sizeof(void*), std::alignment_of<void*>::value>;

            /// \brief Interface to the actual implementation of the deserialization logic.
            /// This interface is used to erase the concrete type of the property.
            struct IContent{

                /// \brief Attempts to deserialize the property of the specified instance via the provided JSON object.
                /// The method does nothing if the provided instance doesn't support the property.
                /// \param instance Instance declaring the property.
                /// \param json JSON object to deserialize.
                virtual bool Deserialize(reflection::Instance instance, const nlohmann::json& json) const = 0;

                /// \brief Clone the content to another instance.
                virtual void Clone(storage_t& storage) const noexcept = 0;

            };

            /// \brief Concrete class implementing the deserialization logic, according to the property type (field, getter or getter/setter pair).
            /// \see Template specialization below.
            template <typename... TAccessors>
            struct Content : IContent {};
            
            storage_t content_;						///< \brief Used to dispatch the deserialization logic.

        };
        
        /// \brief Functor object used to assign the interface JSONDeserializable to properties.
        /// \author Raffaele D. Facendola - September 2016
        struct JSONRead {
 
            /// \brief Add a JSONDeserializable interface to the provided property.
            /// \param property Property to add the interface to.
            /// \param accessors Concrete accessors to the property (such as pointer to member variables, getters and getter/setter pairs)
            template <typename... TAccessors>
            void operator()(reflection::Property& property, TAccessors&&... accessors) const;

        };

        /// \brief Parse a JSON object from file.
        /// \param path Path of the file to parse.
        /// \return Returns a JSON object containing the data stored inside the file.
        nlohmann::json ParseJSONFile(const char* path);

        /// \brief Deserialize a JSON object inside a C++ object.
        ///
        /// (1) If TClass is a pointer, a new object is instantiated with concrete type equal to the type declared by the JSON object. If the method succeeds the object is guaranteed to have a type that can be statically casted to TClass otherwise the returned pointer is nullptr.
        /// (2) If TClass is not a pointer, the concrete type declared by the JSON object is ignored as well as all those properties that are not available in TClass.
        /// (3) The caller takes the *ownership* of all dynamically-allocated objects deserialized this way.
        /// (4) TClass must either be registered to the Syntropy reflection system or a template specialization of JSONDeserializer<TClass> must be provided, otherwise the program is ill-formed.
        /// (5) If the same property is defined more than once, only one of those will be deserialized. Which one is unknown, though.
        ///
        /// \param object Reference to the object to fill with deserialized data.
        /// \param json JSON object to deserialize.
        /// \return Returns true if the JSON object could be deserialized to the provided object, returns false otherwise.
        template <typename TClass>
        bool DeserializeObjectFromJSON(TClass& object, const nlohmann::json& json);

    }

}

namespace syntropy {

    namespace syntax {

        //////////////// JSON DESERIALIZABLE ////////////////

        template <typename... TAccessors>
        JSONDeserializable::JSONDeserializable(property_tag, TAccessors&&... accessors) {

            using content_t = Content<std::remove_reference_t<TAccessors>...>;

            static_assert(sizeof(storage_t) >= sizeof(content_t), "Storage size is not enough.");
        
            new (&content_) content_t(std::forward<TAccessors>(accessors)...);

        }

        template <typename TInstance>
        bool JSONDeserializable::operator()(TInstance&& instance, const nlohmann::json& json) const {

            return reinterpret_cast<const IContent*>(&content_)->Deserialize(reflection::MakeInstance(instance), json);

        }

        //////////////// JSON DESERIALIZABLE :: CONTENT ////////////////

        /// \brief Template specialization of JSONDeserializable::Content for properties defined via their member variable pointer.
        /// \author Raffaele D. Facendola - September 2016.
        template <typename TClass, typename TField>
        struct JSONDeserializable::Content<TField TClass::*> : JSONDeserializable::IContent {

        public:

            Content(TField TClass::* field)
                : field_(field) {}

            virtual bool Deserialize(reflection::Instance instance, const nlohmann::json& json) const override {

                auto concrete_instance = instance.As<TClass>();

                if (concrete_instance) {

                    return JSONDeserializer<TField>()(concrete_instance->*field_, json);

                }

                return false;

            }

            virtual void Clone(storage_t& storage) const noexcept override{

                using this_t = std::remove_reference_t<decltype(*this)>;

                static_assert(sizeof(storage_t) >= sizeof(this_t), "Storage size is not enough for an instance of this class.");

                new (&storage) this_t(field_);

            }

            TField TClass::* field_;                ///< \brief Member field.

        };

        /// \brief Template specialization of JSONDeserializable::Content for properties defined via a pointer to a member function getter.
        /// Read-only properties cannot be deserialized. This specialization does nothing.
        /// \author Raffaele D. Facendola - September 2016.
        template <typename TClass, typename TProperty>
        struct JSONDeserializable::Content<TProperty(TClass::*)() const> : JSONDeserializable::IContent {

            Content(TProperty(TClass::*)() const) {}

            virtual bool Deserialize(reflection::Instance, const nlohmann::json&) const override{

                // Read-only
                return false;

            }

            virtual void Clone(storage_t& storage) const noexcept override{

                using this_t = std::remove_reference_t<decltype(*this)>;

                static_assert(sizeof(storage_t) >= sizeof(this_t), "Storage size is not enough for an instance of this class.");

                new (&storage) this_t(nullptr);

            }

        };

        /// \brief Template specialization of JSONDeserializable::Content for properties defined via a getter/setter member function pointer pair.
        /// The getter has the form - Property GetProperty() const;
        /// THe setter has the form - * SetProperty(Property);
        /// \author Raffaele D. Facendola - September 2016.
        template <typename TClass, typename TProperty, typename TReturn>
        struct JSONDeserializable::Content<TProperty(TClass::*)() const, TReturn(TClass::*)(TProperty)> : JSONDeserializable::IContent {

            Content(TProperty(TClass::*)() const, TReturn(TClass::* setter)(TProperty))
                : setter_(setter) {}

            virtual bool Deserialize(reflection::Instance instance, const nlohmann::json& json) const override{

                auto concrete_instance = instance.As<TClass>();

                if (concrete_instance){

                    // Deserialize the property on a temporary variable and assign that to the setter.

                    std::remove_cv_t<std::remove_reference_t<TProperty>> property;

                    if (JSONDeserializer<TProperty>()(property, json)) {

                        (concrete_instance->*setter_)(std::move(property));

                        return true;

                    }


                }

                return false;

            }

            virtual void Clone(storage_t& storage) const noexcept override {

                using this_t = std::remove_reference_t<decltype(*this)>;

                static_assert(sizeof(storage_t) >= sizeof(this_t), "Storage size is not enough for an instance of this class.");

                new (&storage) this_t(nullptr, setter_);

            }

            TReturn(TClass::* setter_)(TProperty);          ///< \brief Setter method of the property.

        };

        /// \brief Template specialization of JSONDeserializable::Content for properties defined via a getter/setter member function pointer pair.
        /// The getter has the form - const Property& GetProperty() const;
        /// THe setter has the form - Property& GetProperty();
        /// \author Raffaele D. Facendola - September 2016.
        template <typename TClass, typename TProperty>
        struct JSONDeserializable::Content<const TProperty&(TClass::*)() const, TProperty&(TClass::*)()> : JSONDeserializable::IContent {

            Content(const TProperty&(TClass::*)() const, TProperty&(TClass::* setter)())
                : setter_(setter) {}

            virtual bool Deserialize(reflection::Instance instance, const nlohmann::json& json) const override{

                auto concrete_instance = instance.As<TClass>();

                if (concrete_instance){

                    return JSONDeserializer<TProperty>()((concrete_instance->*setter_)(), json);

                }

                return false;

            }

            virtual void Clone(storage_t& storage) const noexcept override{

                using this_t = std::remove_reference_t<decltype(*this)>;

                static_assert(sizeof(storage_t) >= sizeof(this_t), "Storage size is not enough for an instance of this class.");

                new (&storage) this_t(nullptr, setter_);

            }

            TProperty&(TClass::* setter_)();                   ///< \brief Setter method of the property.

        };

        //////////////// JSON WRITE ////////////////

        template <typename... TAccessors>
        void JSONRead::operator()(reflection::Property& property, TAccessors&&... accessors) const {

            property.AddInterface<JSONDeserializable>(JSONDeserializable::property_tag(),
                                                      std::forward<TAccessors>(accessors)...);

        }

        //////////////// METHODS ////////////////

        template <typename TClass>
        bool DeserializeObjectFromJSON(TClass& object, const nlohmann::json& json) {

            static_assert(!std::is_const<std::remove_reference_t<TClass>>::value, "Cannot deserialize into a const object");

            return JSONDeserializer<TClass>()(object, json);

        }

    }

}