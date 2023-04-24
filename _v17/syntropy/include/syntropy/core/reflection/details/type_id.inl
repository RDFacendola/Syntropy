
/// \file type_id.inl
///
/// \author Raffaele D. Facendola - January 2021.

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

    inline Bool
    operator==(Immutable<TypeId> lhs, Immutable<TypeId> rhs) noexcept
    {
        return (lhs.type_index_ == rhs.type_index_);
    }

    inline Ordering
    operator<=>(Immutable<TypeId> lhs, Immutable<TypeId> rhs) noexcept
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
