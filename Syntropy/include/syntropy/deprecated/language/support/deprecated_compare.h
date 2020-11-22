
/// \file deprecated_compare.h
/// \brief This header is part of the Syntropy deprecated module. It contains deprecated functionalities in compare.h.
///
/// \author Raffaele D. Facendola - November 2020

#pragma once

#include "syntropy/language/foundation/foundation.h"

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* LEGACY                                                               */
    /************************************************************************/

    /// \brief Stream insertion for StrongOrdering.
    std::ostream& operator<<(std::ostream& out, Reference<StrongOrdering> rhs)
    {
        if (rhs == StrongOrdering::Equal)
        {
            out << "Equal";
        }
        else if (rhs == StrongOrdering::Less)
        {
            out << "Less";
        }
        else if (rhs == StrongOrdering::Greater)
        {
            out << "Greater";
        }
        else
        {
            out << "Unknown StrongOrdering value";
        }

        return out;
    }

    /// \brief Stream insertion for StrongOrdering.
    std::ostream& operator<<(std::ostream& out, Reference<WeakOrdering> rhs)
    {
        if (rhs == WeakOrdering::Equivalent)
        {
            out << "Equal";
        }
        else if (rhs == WeakOrdering::Less)
        {
            out << "Less";
        }
        else if (rhs == WeakOrdering::Greater)
        {
            out << "Greater";
        }
        else
        {
            out << "Unknown WeakOrdering value";
        }

        return out;
    }

    /// \brief Stream insertion for StrongOrdering.
    std::ostream& operator<<(std::ostream& out, Reference<PartialOrdering> rhs)
    {
        if (rhs == PartialOrdering::Equivalent)
        {
            out << "Equal";
        }
        else if (rhs == PartialOrdering::Less)
        {
            out << "Less";
        }
        else if (rhs == PartialOrdering::Greater)
        {
            out << "Greater";
        }
        else if (rhs == PartialOrdering::Unordered)
        {
            out << "Incomparable";
        }
        else
        {
            out << "Unknown PartialOrdering value";
        }

        return out;
    }
}

// ===========================================================================