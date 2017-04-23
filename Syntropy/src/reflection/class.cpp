#include "reflection/class.h"

#include "reflection/reflection.h"

#include <algorithm>

namespace syntropy
{

    namespace reflection 
    {

        /************************************************************************/
        /* CLASS                                                                */
        /************************************************************************/

        bool Class::operator ==(const Class& other) const noexcept
        {
            if (this == std::addressof(other))
            {
                return true;        // The two classes point to the same singleton instance.
            }

            // Check whether any base classes of this class is the same as the other.
            return std::any_of
            (
                base_classes_.begin(),
                base_classes_.end(),
                [&other](const Class* base_class)
                {
                    return *base_class == other;
                }
            );

        }

        bool Class::operator !=(const Class& other) const noexcept
        {
            return !(*this == other);
        }

        const HashedString& Class::GetDefaultName() const noexcept
        {
            return default_name_;
        }

        const std::vector<HashedString>& Class::GetNameAliases() const noexcept
        {
            return name_aliases_;
        }

        const std::vector<const Class*>& Class::GetBaseClasses() const noexcept
        {
            return base_classes_;
        }

        const Property* Class::GetProperty(const HashedString& property_name) const noexcept
        {
            auto it = std::find_if
            (
                properties_.begin(),
                properties_.end(),
                [&property_name](const Property& property)
                {
                    return property.GetName() == property_name;
                }
            );

            return it != properties_.end() ?
                std::addressof(*it) :
                nullptr;
        }

        const std::vector<Property>& Class::GetProperties() const noexcept
        {
            return properties_;
        }

        bool Class::IsAbstract() const noexcept
        {
            return is_abstract_;
        }

        std::ostream& operator<<(std::ostream& out, const Class& class_instance)
        {
            out << class_instance.GetDefaultName();

            return out;
        }

    }

}


