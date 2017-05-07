#include "serialization/json.h"

#include <fstream>

#include "reflection/class_interfaces.h"

#include "diagnostics/log.h"

namespace syntropy 
{
    namespace serialization 
    {

        /************************************************************************/
        /* JSON DESERIALIZABLE                                                  */
        /************************************************************************/

        JSONDeserializable::JSONDeserializable(const JSONDeserializable& other) noexcept
        {
            reinterpret_cast<const IContent*>(&(other.content_))->Clone(content_);
        }

        JSONDeserializable::JSONDeserializable(JSONDeserializable&& other) noexcept
            : content_(std::move(other.content_))
        {

        }

        JSONDeserializable& JSONDeserializable::operator=(JSONDeserializable other) noexcept
        {
            JSONDeserializable(other).Swap(*this);

            return *this;
        }

        void JSONDeserializable::Swap(JSONDeserializable& other) noexcept
        {
            std::swap(content_, other.content_);
        }

        JSONDeserializable::~JSONDeserializable()
        {
            reinterpret_cast<IContent*>(&(content_))->~IContent();
        }

        /************************************************************************/
        /* JSON CONSTRUCTIBLE                                                   */
        /************************************************************************/

        reflection::Any JSONConstructible::operator()(const nlohmann::json& /*json*/) const
        {
            return reflection::Any();
        }

    }

}
