#include "serialization/json.h"

namespace syntropy {

	namespace serialization {

		JSONDeserializable::JSONDeserializable(const JSONDeserializable& other) noexcept
			: content_(other.content_ ? other.content_->Clone() : nullptr) {}

		JSONDeserializable::JSONDeserializable(JSONDeserializable&& other) noexcept
			: content_(std::move(other.content_)) {}

		JSONDeserializable& JSONDeserializable::operator=(JSONDeserializable other) noexcept {

			JSONDeserializable(other).Swap(*this);

			return *this;

		}

		void JSONDeserializable::Swap(JSONDeserializable& other) noexcept {

			std::swap(content_, other.content_);

		}

	}

}