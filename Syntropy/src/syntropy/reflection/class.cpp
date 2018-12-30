#include "syntropy/reflection/class.h"

#include "syntropy/reflection/reflection.h"

#include <algorithm>

namespace syntropy::reflection
{
    /************************************************************************/
    /* CLASS                                                                */
    /************************************************************************/

    void Class::RegisterClass()
    {
        Reflection::GetInstance().RegisterClass(*this);
    }

    bool Class::IsA(const Class& other) const noexcept
    {
        return (*this == other) || std::any_of(base_classes_.begin(), base_classes_.end(), [&other](const auto& base_class)
        {
            return base_class->IsA(other);
        });
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
        auto it = std::find_if(properties_.begin(), properties_.end(), [&property_name](const auto& property)
        {
            return property.GetName() == property_name;
        });

        return it != properties_.end() ? std::addressof(*it) : nullptr;
    }

    const std::vector<Property>& Class::GetProperties() const noexcept
    {
        return properties_;
    }

    const std::type_index& Class::GetTypeIndex() const noexcept
    {
        return type_index_;
    }

    bool Class::IsAbstract() const noexcept
    {
        return is_abstract_;
    }

    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/

    bool operator ==(const Class& lhs, const Class& rhs) noexcept
    {
        return lhs.GetDefaultName() == rhs.GetDefaultName();
    }

    bool operator !=(const Class& lhs, const Class& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    std::ostream& operator<<(std::ostream& out, const Class& class_t)
    {
        out << class_t.GetDefaultName();

        return out;
    }

}


