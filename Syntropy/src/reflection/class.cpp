#include "reflection/class.h"

#include "reflection/reflection.h"

#include <algorithm>

namespace syntropy::reflection
{
    /************************************************************************/
    /* CLASS                                                                */
    /************************************************************************/

    Class::Class(tag_t<void>)
        : default_name_("void")
        , is_abstract_(true)
    {
        // Register the class to the reflection system.
        Reflection::GetInstance().Register(*this);
    }

    bool Class::operator ==(const Class& other) const noexcept
    {
        return this == std::addressof(other);
    }

    bool Class::operator !=(const Class& other) const noexcept
    {

        return this != std::addressof(other);
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

    bool Class::IsAbstract() const noexcept
    {
        return is_abstract_;
    }

    void Class::AddNameAlias(HashedString name_alias)
    {
        if (std::find(name_aliases_.begin(), name_aliases_.end(), name_alias) == name_aliases_.end())
        {
            name_aliases_.push_back(name_alias);
        }
    }

    void Class::AddBaseClass(const Class& base_class)
    {
        if (std::find(base_classes_.begin(), base_classes_.end(), &base_class) == base_classes_.end())
        {
            base_classes_.push_back(&base_class);
        }
    }

    std::ostream& operator<<(std::ostream& out, const Class& class_instance)
    {
        out << class_instance.GetDefaultName();

        return out;
    }
}


