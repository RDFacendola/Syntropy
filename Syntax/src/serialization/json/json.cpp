#include "serialization/json.h"

#include <fstream>

namespace syntropy {

    namespace syntax {

        namespace serialization {

            //////////////// JSON DESERIALIZABLE ////////////////

            const char* JSONDeserializable::kClassToken = "$class";

            JSONDeserializable::JSONDeserializable(const JSONDeserializable& other) noexcept {

                reinterpret_cast<const IContent*>(&(other.content_))->Clone(content_);

            }
        
            JSONDeserializable::JSONDeserializable(JSONDeserializable&& other) noexcept
                : content_(std::move(other.content_)) {}

            JSONDeserializable& JSONDeserializable::operator=(JSONDeserializable other) noexcept {

                JSONDeserializable(other).Swap(*this);

                return *this;

            }

            void JSONDeserializable::Swap(JSONDeserializable& other) noexcept {

                std::swap(content_, other.content_);

            }

            JSONDeserializable::~JSONDeserializable(){

                reinterpret_cast<IContent*>(&(content_))->~IContent();
            
            }

            //////////////// METHODS ////////////////

            nlohmann::json ParseJSONFile(const char* path) {

                std::fstream file_stream(path);

                nlohmann::json json;

                if (file_stream.good()) {

                    file_stream >> json;

                }

                return json;

            }

        }

    }

}