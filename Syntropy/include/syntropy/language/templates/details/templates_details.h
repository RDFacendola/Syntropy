
/// \file templates_details.h
/// \brief This header is part of Syntropy language module. It contains implementation details of basic templates machinery.
///
/// \author Raffaele D. Facendola - 2020.

#pragma once

// ===========================================================================

namespace Syntropy::Templates::Details
{
    /************************************************************************/
    /* IDENTITY                                                             */
    /************************************************************************/

    /// \brief Exposes a member type equal to TType.
    /// \remarks The identity transform is meant to establish non-deduced contexts in template argument deduction.
    template <typename TType>
    struct Identity
    {
        using Type = TType;
    };

}

// ===========================================================================