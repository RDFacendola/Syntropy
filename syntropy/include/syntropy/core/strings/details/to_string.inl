
/// \file to_string.inl
///
/// \author Raffaele D. Facendola - February 2021

// ===========================================================================

namespace Syntropy::Strings
{
    /************************************************************************/
    /* NON-MEMBER FUNCTIONS                                                 */
    /************************************************************************/

    // ToString.
    // =========

    /// \brief Convert rhs to a string.
    template <typename TType>
    inline auto
    ToString(Immutable<TType> rhs) noexcept
        -> decltype(Details::RouteToString(rhs))
    {
        return Details::RouteToString(rhs);
    }

}

// ===========================================================================
