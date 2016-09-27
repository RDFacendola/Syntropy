/// \file json.h
/// \brief TODO: Add brief description here
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <fstream>
#include <memory>
#include <functional>

#include "reflection/reflection.h"

#include "serialization/json/json_deserializer.h"

namespace syntropy {

    namespace serialization {

        class JSONDeserializable{

        public:

			template <typename... TAccessors>
			JSONDeserializable(TAccessors&&... /*accessors*/)
				: content_(nullptr/*std::make_unique<Content<std::decay_t<TAccessors>...>>(std::forward<TAccessors>(accessors)...)*/)
			{
			
				Content<TAccessors...> content;

				SYN_UNUSED(content);

			
			}

			JSONDeserializable(const JSONDeserializable& other) noexcept
				: content_(other.content_->Clone())
			{}

			JSONDeserializable(JSONDeserializable&& other) noexcept
				: content_(std::move(other.content_))
			{}

			JSONDeserializable::~JSONDeserializable() = default;

			JSONDeserializable& operator=(JSONDeserializable other) noexcept{

				JSONDeserializable(other).Swap(*this);

				return *this;

			}

            template <typename TInstance>
            void operator()(TInstance&& instance, const nlohmann::json& json) const {

                content_->Deserialize(reflection::MakeInstance(instance), json);

            }

		private:

			void Swap(JSONDeserializable& other) noexcept{

				std::swap(content_, other.content_);

			}

			struct IContent{

				virtual void Deserialize(reflection::Instance instance, const nlohmann::json& json) const = 0;

				virtual std::unique_ptr<IContent> Clone() const noexcept = 0;

			};

			template <typename... TAccessors>
			struct Content : IContent {};

			std::unique_ptr<IContent> content_;

        };
		
        // Property interface

        struct JsonDeserializable {
 
            template <typename... TAccessors>
            void operator()(reflection::Property& property, TAccessors... accessors) const {

                property.AddInterface<JSONDeserializable, TAccessors...>(std::forward<TAccessors>(accessors)...);

            }

        };

        // Utilities

        inline nlohmann::json ParseJSON(const char* path) {

            std::fstream file_stream(path);

            nlohmann::json json;

            if (file_stream.good()) {

                file_stream >> json;

            }

            return json;

        }

        template <typename TClass>
        bool DeserializeJSON(TClass& object, const nlohmann::json& json){

            JsonDeserializer<TClass>()(object, json);

            return true;

        }

        template <typename TClass>
        std::unique_ptr<TClass> DeserializeJSON(const nlohmann::json& json){

            TClass* object;

            JsonDeserializer<TClass>()(object, json);

            return object;

        }

        template <typename TClass>
        bool DeserializeJSON(TClass& object, const char* path) {

            return DeserializeJSON(object, ParseJSON(path));
            
        }

        template <typename TClass>
        std::unique_ptr<TClass> DeserializeJSON(const char* path) {

            return DeserializeJSON(ParseJSON(path));

        }
        
    }

}

namespace syntropy {

    namespace serialization {

		template <typename TClass, typename TField>
		struct JSONDeserializable::Content<TField TClass::*> : JSONDeserializable::IContent
		{

		public:

			Content(TField TClass::* field)
				: field_(field)
			{
			}

			virtual void Deserialize(reflection::Instance instance, const nlohmann::json& json) const override
			{

				auto concrete_instance = instance.As<TClass>();

				if (concrete_instance)
				{

					JsonDeserializer<TField>()(concrete_instance->*field_, json);

				}

			}

			virtual std::unique_ptr<IContent> Clone() const noexcept override{
			
				return std::make_unique<Content<TField TClass::*>>(field_);

			}

			TField TClass::* field_;                ///< \brief Member field.

		};

		template <typename TClass, typename TProperty>
		struct JSONDeserializable::Content<TProperty(TClass::*)() const> : JSONDeserializable::IContent
		{

			Content(TProperty(TClass::*)() const) {}

			virtual void Deserialize(reflection::Instance, const nlohmann::json&) const override
			{

				// Read-only

			}

			virtual std::unique_ptr<IContent> Clone() const noexcept override
			{

				return std::make_unique<Content<TProperty(TClass::*)() const>>();

			}

		};

		template <typename TClass, typename TProperty, typename TReturn>
		struct JSONDeserializable::Content<TProperty(TClass::*)() const, TReturn(TClass::*)(TProperty)> : JSONDeserializable::IContent
		{

			Content(TProperty(TClass::*)() const, TReturn(TClass::* setter)(TProperty))
				: setter_(setter)
			{
			}

			virtual void Deserialize(reflection::Instance instance, const nlohmann::json& json) const override
			{

				auto concrete_instance = instance.As<TClass>();

				if (concrete_instance)
				{

					// Deserialize the property on a temporary variable and assign that to the setter.
					std::remove_cv_t<std::remove_reference_t<TProperty>> property;

					JsonDeserializer<TProperty>()(property, json);

					(concrete_instance->*setter_)(std::move(property));

				}

			}

			virtual std::unique_ptr<IContent> Clone() const noexcept override
			{

				return std::make_unique<Content<TProperty(TClass::*)() const, TReturn(TClass::*)(TProperty)>>(setter_);

			}

			TReturn(TClass::* setter_)(TProperty);          ///< \brief Setter method of the property.

		};

		template <typename TClass, typename TProperty>
		struct JSONDeserializable::Content<const TProperty&(TClass::*)() const, TProperty&(TClass::*)()> : JSONDeserializable::IContent
		{

			Content(const TProperty&(TClass::*)() const, TProperty&(TClass::* setter)())
				: setter_(setter)
			{
			}

			virtual void Deserialize(reflection::Instance instance, const nlohmann::json& json) const override
			{

				auto concrete_instance = instance.As<TClass>();

				if (concrete_instance)
				{

					JsonDeserializer<TProperty>()((concrete_instance->*setter_)(), json);

				}

			}

			virtual std::unique_ptr<IContent> Clone() const noexcept override
			{

				return std::make_unique<Content<const TProperty&(TClass::*)() const, TProperty&(TClass::*)()>>(setter_);

			}

			TProperty&(TClass::* setter_)();                   ///< \brief Setter method of the property.

		};

    }

}