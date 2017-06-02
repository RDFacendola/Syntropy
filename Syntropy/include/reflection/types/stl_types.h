
/// \file stl_types.h
/// \brief This header is part of the syntropy reflection system. It contains the definition of all the SLT types.
///
/// \author Raffaele D. Facendola - 2016

#pragma once

#include <functional>

#include <type_traits>
#include <string>
#include <memory>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include "reflection/class.h"

namespace syntropy
{
    namespace reflection
    {

        /************************************************************************/
        /* NULLPTR                                                              */
        /************************************************************************/
        template <>
        struct ClassDeclaration<std::nullptr_t>
        {
            static constexpr const char* GetName() noexcept 
            {
                return "std::nullptr_t";
            }
        };

        /************************************************************************/
        /* VECTOR                                                               */
        /************************************************************************/

        template <typename TType>
        struct ClassDeclaration<std::vector<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::vector"));
                return name.c_str();
            }
        };

        /************************************************************************/
        /* PAIR                                                                 */
        /************************************************************************/

        template <typename TFirst, typename TSecond>
        struct ClassDeclaration<std::pair<TFirst, TSecond> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TFirst, TSecond>("std::pair"));
                return name.c_str();
            }
        };

        /************************************************************************/
        /* SETS                                                                 */
        /************************************************************************/

        template <typename TType>
        struct ClassDeclaration<std::set<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::set"));
                return name.c_str();
            }
        };

        template <typename TType>
        struct ClassDeclaration<std::unordered_set<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::unordered_set"));
                return name.c_str();
            }
        };

        template <typename TType>
        struct ClassDeclaration<std::multiset<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::multiset"));
                return name.c_str();
            }
        };

        template <typename TType>
        struct ClassDeclaration<std::unordered_multiset<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::unordered_multiset"));
                return name.c_str();
            }
        };

        /************************************************************************/
        /* MAPS                                                                 */
        /************************************************************************/

        template <typename TKey, typename TValue>
        struct ClassDeclaration<std::map<TKey, TValue> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TKey, TValue>("std::map"));
                return name.c_str();
            }
        };

        template <typename TKey, typename TValue>
        struct ClassDeclaration<std::unordered_map<TKey, TValue> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TKey, TValue>("std::unordered_map"));
                return name.c_str();
            }
        };

        template <typename TKey, typename TValue>
        struct ClassDeclaration<std::multimap<TKey, TValue> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TKey, TValue>("std::multimap"));
                return name.c_str();
            }
        };

        template <typename TKey, typename TValue>
        struct ClassDeclaration<std::unordered_multimap<TKey, TValue> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TKey, TValue>("std::unordered_multimap"));
                return name.c_str();
            }
        };

        /************************************************************************/
        /* STRING                                                               */
        /************************************************************************/

        template <>
        struct ClassDeclaration<std::string>
        {
            static constexpr const char* GetName() noexcept
            {
                return "std::string";
            }
        };

        template <>
        struct ClassDeclaration<std::wstring>
        {
            static constexpr const char* GetName() noexcept
            {
                return "std::wstring";
            }
        };

        template <typename TType>
        struct ClassDeclaration<std::char_traits<TType>>
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::char_traits"));
                return name.c_str();
            }
        };

        /************************************************************************/
        /* SMART POINTERS                                                       */
        /************************************************************************/

        template <typename TType>
        struct ClassDeclaration<std::unique_ptr<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::unique_ptr"));
                return name.c_str();
            }
        };

        template <typename TType>
        struct ClassDeclaration<std::shared_ptr<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::shared_ptr"));
                return name.c_str();
            }
        };

        /************************************************************************/
        /* ALLOCATION                                                           */
        /************************************************************************/

        template <typename TType>
        struct ClassDeclaration<std::allocator<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::allocator"));
                return name.c_str();
            }
        };

        template <typename TType>
        struct ClassDeclaration<std::default_delete<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::default_delete"));
                return name.c_str();
            }
        };

        /************************************************************************/
        /* HASHING                                                              */
        /************************************************************************/

        template <typename TType>
        struct ClassDeclaration<std::hash<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::hash"));
                return name.c_str();
            }
        };

        /************************************************************************/
        /* COMPARISON                                                           */
        /************************************************************************/

        template <typename TType>
        struct ClassDeclaration<std::equal_to<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::equal_to"));
                return name.c_str();
            }
        };

        template <typename TType>
        struct ClassDeclaration<std::not_equal_to<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::not_equal_to"));
                return name.c_str();
            }
        };

        template <typename TType>
        struct ClassDeclaration<std::greater<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::greater"));
                return name.c_str();
            }
        };

        template <typename TType>
        struct ClassDeclaration<std::less<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::less"));
                return name.c_str();
            }
        };

        template <typename TType>
        struct ClassDeclaration<std::greater_equal<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::greater_equal"));
                return name.c_str();
            }
        };

        template <typename TType>
        struct ClassDeclaration<std::less_equal<TType> >
        {
            static const char* GetName() noexcept
            {
                static std::string name(GenerateClassName<TType>("std::less_equal"));
                return name.c_str();
            }
        };

    }

}
