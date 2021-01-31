
/// \file type_id.h
/// \brief This header is part of the Syntropy core module. It contains definitions for type identifiers.
///
/// \author Raffaele D. Facendola - January 2021

#pragma once

#include <typeinfo>
#include <typeindex>

#include "syntropy/language/foundation/foundation.h"
#include "syntropy/language/support/compare.h"

// ===========================================================================

namespace Syntropy::Reflection
{
    /************************************************************************/
    /* TYPE ID                                                              */
    /************************************************************************/

    /// \brief Opaque object used to identify a type.
    /// \remarks Type ids account for reference and pointers types, not for qualifiers such as "const" or "volatile".
    /// \author Raffaele D. Facendola - January 2021
    class TypeId
    {

        friend Bool operator==(Immutable<TypeId> lhs, Immutable<TypeId> rhs) noexcept;

        friend Ordering operator<=>(Immutable<TypeId> lhs, Immutable<TypeId> rhs) noexcept;

        template <typename TType>
        friend TypeId TypeIdOf() noexcept;

    public:

        /// \brief Default constructor.
        /// Default-constructed TypeId refers to the type "void".
        TypeId() noexcept;

        /// \brief Create a type id from the dynamic type of rhs.
        template <typename TType>
        TypeId(Immutable<TType> rhs) noexcept;

    private:

        /// \brief Create a new type info.
        TypeId(Immutable<std::type_info> type_info) noexcept;

        /// \brief Native type info.
        std::type_index type_index_;

    };

    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // Comparison.
    // ===========

    /// \brief Check whether lhs and rhs both refers to the same type.
    Bool operator==(Immutable<TypeId> lhs, Immutable<TypeId> rhs) noexcept;
    
    /// \brief Compare lhs and rhs.
    Ordering operator<=>(Immutable<TypeId> lhs, Immutable<TypeId> rhs) noexcept;

    // Utilities.
    // ==========

    /// \brief Get the dynamic type id of rhs.
    /// \remarks If TType is not polymorphic, the returned value is equal to its static type id.
    template <typename TType>
    TypeId TypeIdOf(Immutable<TType> rhs) noexcept;

    /// \brief Get the static type id of rhs.
    template <typename TType>
    TypeId TypeIdOf() noexcept;

}

// ===========================================================================

namespace Syntropy::Reflection
{
    /************************************************************************/
    /* IMPLEMENTATION                                                       */
    /************************************************************************/

    // TypeId.
    // =======

    inline TypeId::TypeId() noexcept
        : TypeId(typeid(void))
    {

    }

    template <typename TType>
    inline TypeId::TypeId(Immutable<TType> rhs) noexcept
        : TypeId(typeid(rhs))
    {

    }

    inline TypeId::TypeId(Immutable<std::type_info> type_info) noexcept
        : type_index_(type_info)
    {

    }

    // Non-member functions.
    // =====================

    inline Bool operator==(Immutable<TypeId> lhs, Immutable<TypeId> rhs) noexcept
    {
        return (lhs.type_index_ == rhs.type_index_);
    }

    inline Ordering operator<=>(Immutable<TypeId> lhs, Immutable<TypeId> rhs) noexcept
    {
        if (lhs.type_index_ < rhs.type_index_)
        {
            return Ordering::kLess;
        }

        if (lhs.type_index_ > rhs.type_index_)
        {
            return Ordering::kGreater;
        }

        return Ordering::kEquivalent;
    }

    template <typename TType>
    inline TypeId TypeIdOf(Immutable<TType> rhs) noexcept
    {
        return TypeId{ rhs };
    }

    template <typename TType>
    inline TypeId TypeIdOf() noexcept
    {
        return TypeId{ typeid(TType) };
    }

}

// ===========================================================================