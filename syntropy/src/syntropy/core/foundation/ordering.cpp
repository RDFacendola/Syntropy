
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

    Immutable<String>
    ToString(Immutable<Ordering> rhs) noexcept
    {
        static const auto kLess
            = String{u8"Syntropy::Ordering::kLess"};

        static const auto kEquivalent
            = String{u8"Syntropy::Ordering::kEquivalent"};

        static const auto kGreater
            = String{u8"Syntropy::Ordering::kGreater"};

        static const auto kIncomparable
            = String{u8"Syntropy::Ordering::kIncomparable"};

        if(rhs == Ordering::kLess)
        {
            return kLess;
        }

        if(rhs == Ordering::kEquivalent)
        {
            return kEquivalent;
        }

        if(rhs == Ordering::kGreater)
        {
            return kGreater;
        }

        return kIncomparable;
    }

}

// ===========================================================================
