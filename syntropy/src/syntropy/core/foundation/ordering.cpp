
/// \file ordering.cpp
///
/// \author Raffaele D. Facendola - April 2021

// ===========================================================================

#include "syntropy/core/strings/string.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* ORDERING                                                             */
    /************************************************************************/

    Immutable<String> ToString(Immutable<Ordering> rhs) noexcept
    {
        static const auto kLess = String{u8"Less"};
        static const auto kEquivalent = String{u8"Equivalent"};
        static const auto kGreater = String{u8"Greater"};

        if(rhs == Ordering::kLess)
        {
            return kLess;
        }

        if(rhs == Ordering::kEquivalent)
        {
            return kEquivalent;
        }

        return kGreater;
    }

}

// ===========================================================================
