#include "reflection/reflection.h"

#include "reflection/class.h"

#include "diagnostics/log.h"

namespace syntropy
{
    namespace reflection
    {
        const diagnostics::Context ReflectionCtx("SyntropyReflection");

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
            class_aliases_.reserve(1024);
        }

        const Class* Reflection::GetClass(const HashedString& class_name) const noexcept
        {
            auto it = default_classes_.find(class_name);

            if (it != default_classes_.end())
            {
                return it->second;
            }
            else
            {
                it = class_aliases_.find(class_name);

                return (it != class_aliases_.end())
                    ? it->second
                    : nullptr;
            }
        }

        void Reflection::Register(Class& class_instance)
        {
            // Register the default class name

            auto it = default_classes_.find(class_instance.GetDefaultName());

            if (it == default_classes_.end())
            {
                default_classes_.emplace(std::make_pair(class_instance.GetDefaultName(), &class_instance));
            }
            else
            {
                SYNTROPY_ERROR((ReflectionCtx), "A class with default name '", it->first, "' already exists. The new instance has been ignored.");
            }

            // Register each alias as a different entry

            for (auto&& name_alias : class_instance.GetNameAliases())
            {
                it = class_aliases_.find(name_alias);

                if (it == class_aliases_.end())
                {
                    class_aliases_.emplace(std::make_pair(name_alias, &class_instance));
                }
                else
                {
                    SYNTROPY_WARNING((ReflectionCtx), "A class with name alias '", it->first, "' already exists. The name alias has been invalidated.");
                    it->second = nullptr;
                }
            }

        }

        /************************************************************************/
        /* METHODS                                                              */
        /************************************************************************/

        const Class* GetClass(const HashedString& class_name) noexcept
        {
            return Reflection::GetInstance().GetClass(class_name);
        }

    }
}

