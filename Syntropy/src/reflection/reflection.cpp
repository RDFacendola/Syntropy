#include "reflection/reflection.h"

#include "reflection/class.h"

#include "diagnostics/log.h"

namespace syntropy::reflection
{
    /************************************************************************/
    /* REFLECTION                                                           */
    /************************************************************************/

    Reflection& Reflection::GetInstance() noexcept
    {
        static Reflection instance;

        return instance;
    }

    Reflection::Reflection()
    {
        default_classes_.reserve(1024);
        aliases_classes_.reserve(1024);
    }

    const Class* Reflection::GetClass(const HashedString& class_name) const noexcept
    {
        if (auto it = default_classes_.find(class_name); it != default_classes_.end())
        {
            return it->second;
        }
        else
        {
            it = aliases_classes_.find(class_name);

            return (it != aliases_classes_.end()) ? it->second : nullptr;
        }
    }

    const Class* Reflection::GetClass(const std::type_index& type_index) const noexcept
    {
        auto it = typeindex_classes_.find(type_index);

        return it != std::end(typeindex_classes_) ? it->second : nullptr;
    }

    void Reflection::RegisterClass(const Class& class_t)
    {
        // #TODO Check if the same class was registered more than once (and ignore the rest of the method)

        // Register the default class name.

        if (auto it = default_classes_.find(class_t.GetDefaultName()); it == default_classes_.end())
        {
            default_classes_.emplace(std::make_pair(class_t.GetDefaultName(), &class_t));
        }
        else
        {
            SYNTROPY_ERROR((ReflectionCtx), "A class with default name '", it->first, "' already exists. The new instance has been ignored.");
        }

        // Register each alias as a different entry.

        for (auto&& name_alias : class_t.GetNameAliases())
        {
            if (auto it = aliases_classes_.find(name_alias); it == aliases_classes_.end())
            {
                aliases_classes_.emplace(std::make_pair(name_alias, &class_t));
            }
            else
            {
                SYNTROPY_WARNING((ReflectionCtx), "A class with name alias '", it->first, "' already exists. The name alias has been invalidated.");
                it->second = nullptr;
            }
        }

        // Register the class type index.

        if (auto it = typeindex_classes_.find(class_t.GetTypeIndex()); it == typeindex_classes_.end())
        {
            typeindex_classes_.emplace(std::make_pair(class_t.GetTypeIndex(), &class_t));
        }
        else
        {
            SYNTROPY_ERROR((ReflectionCtx), "A class with type info '", it->first.name(), "' already exists. The new instance has been ignored.");
        }
    }

    /************************************************************************/
    /* METHODS                                                              */
    /************************************************************************/

    const Class* GetClass(const HashedString& class_name) noexcept
    {
        return Reflection::GetInstance().GetClass(class_name);
    }

    const Class* GetClass(const std::type_info& type_info) noexcept
    {
        return Reflection::GetInstance().GetClass(type_info);
    }
}

